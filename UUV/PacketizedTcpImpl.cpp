#include "stdafx.h"
#include "PacketizedTcpImpl.h"
#include <mstcpip.h>

// ���������� (1024*8) ֮����Ϊʲô����8K��Ҳ��һ�������ϵľ���ֵ
// ���ȷʵ�ͻ��˷�����ÿ�����ݶ��Ƚ��٣���ô�����õ�СһЩ��ʡ�ڴ�
const int MAX_BUFFER_LEN = 512;//houmb

/*
���ݰ��ṹ˵��
�ֽ����			��������˵��				˵��
0��1��2��3		����֤��־				Ĭ�ϰ���֤��־Ϊ 0x5678
4��5��6��7		��ʵ�����ݵĴ�С			��������Ϊ�Ӱ�ָ���ֽڵ���һ���ֽڿ�ʼ������β�������ֽ���
*/
const int MAX_PACKET_BODY_LEN = 1024 * 1024;
const int PACKET_HEADER_LEN = 8;
const DWORD PACKET_FLAG = 0x5678;

CPacketizedTcpClient* g_tcpClient = nullptr;

//////////////////////////////////////////////////////////////////////////
//	Client
CPacketizedTcpClient::CPacketizedTcpClient()
{
	g_tcpClient = this;

	m_netState = E_NET_DISCONNED;

	m_bManualDisconn = true;
	m_pClientEvent = NULL;
	m_pStreamSocket = NULL;
	datapackage = (unsigned char *)malloc(DATA_TOTAL_LEN);

	m_taskmanager.cancelAll();
	m_taskmanager.start(new CNetReconnTask("ReconnTask"));
}

CPacketizedTcpClient::~CPacketizedTcpClient()
{
	m_pClientEvent = NULL;
	this->Disconnect();
	while (m_thread.isRunning())
	{
		Sleep(10);
	}
	
	if(datapackage!=NULL)
	{
		free(datapackage);
		datapackage = NULL;
	}

	m_taskmanager.cancelAll();
	//delete this;
}

bool CPacketizedTcpClient::Connect(const char* szHostAddress, unsigned short unPort)
{
	try
	{
		m_netState = E_NET_CONNING;
		if (m_pStreamSocket)	/*	�Ѵ�ֱ�ӷ���	*/
		{
			m_netState = E_NET_CONNED;
			return true;
		}

		m_sockAddr = SocketAddress(szHostAddress, unPort);
		m_pStreamSocket = new StreamSocket();//(m_sockAddr);
		if (!m_pStreamSocket)
		{
			LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("����SOCKETʧ��"));
			return false;
		}
		// �������ӳ�ʱʱ��
		Timespan timeout(1, 0);
		m_pStreamSocket->connect(m_sockAddr, timeout);
		m_pStreamSocket->setKeepAlive(true);
		struct tcp_keepalive keepin;
		struct tcp_keepalive keepout;
		keepin.onoff = 1;
		// ����30Sû�����ݣ��ͷ���̽���
		keepin.keepalivetime = 30*1000;
		// ÿ1S����1��̽�ⱨ�ģ���10��û�л�Ӧ���ͶϿ�
		keepin.keepaliveinterval = 1000;
		DWORD dwBytes = 0;
		int nRet = ::WSAIoctl(m_pStreamSocket->impl()->sockfd(), SIO_KEEPALIVE_VALS, &keepin, sizeof(keepin),
			&keepout, sizeof(keepout), &dwBytes, NULL, NULL);
		if (nRet == SOCKET_ERROR)
		{
			LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("WSAIoctl failed: %d"), WSAGetLastError());
			return false;
		}

		m_thread.start(*this);

		m_netState =  E_NET_CONNED;

		FastMutex::ScopedLock lock(m_mciMutex);
		m_bManualDisconn = true;
		
		return true;
	}
	catch (Poco::Exception& exc)
	{
		if (m_pClientEvent != NULL)
		{
			m_pClientEvent->OnError(exc.displayText());

			FastMutex::ScopedLock lock(m_mciMutex);
			m_bManualDisconn = false;
		}
		Disconnect();
	}
	m_netState = E_NET_DISCONNED;
	return false;
}

void CPacketizedTcpClient::Disconnect()
{
	m_netState = E_NET_DISCONNING;
	if (m_pStreamSocket != NULL)
	{
		m_pStreamSocket->close();
		delete m_pStreamSocket;
		m_pStreamSocket = NULL;
	}
	m_netState = E_NET_DISCONNED;

	listqueue.clearQueue();
}

void CPacketizedTcpClient::SetClientListener(IPacketizedTcpClientEvent* pClientEvent)
{
	m_pClientEvent = pClientEvent;
}

bool CPacketizedTcpClient::Send()
{
	if (m_pStreamSocket != NULL)
	{
		try
		{
			/*CByteStream packetStream;
			packetStream.Write(PACKET_FLAG);
			packetStream.Write(byteStream.GetNumberOfBytesUsed());
			packetStream.Write(byteStream.GetData(), byteStream.GetNumberOfBytesUsed());*/
			unsigned char *pbyteStream=NULL;
			int Datelen=0;
			m_pClientEvent->OnGetSendPacket(&pbyteStream,&Datelen);
			if((pbyteStream!=NULL)&&(Datelen==ROV_SEND_DATALEN))
			{
				m_pStreamSocket->sendBytes(pbyteStream,Datelen);
			}
			else
			{
				LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("���������쳣"));
			}
			return true;
		}
		catch (Poco::Exception& exc)
		{
			if (m_pClientEvent != NULL)
			{
				m_pClientEvent->OnError(exc.displayText());
			}
		}
	}
	return false;
}

void CPacketizedTcpClient::run()
{
	try
	{
		while (m_pStreamSocket != NULL)
		{
			char szBuffer[MAX_BUFFER_LEN]={0};
			int nRcved = m_pStreamSocket->receiveBytes(szBuffer, sizeof(szBuffer));
			if (nRcved <= 0)
			{
				this->Disconnect();
				if (m_pClientEvent != NULL)
				{
					m_pClientEvent->OnCloseConnection();
				}
				FastMutex::ScopedLock lock(m_mciMutex);
				m_bManualDisconn = false;
				break;
			}
			else
			{
				for(int i=0;i<nRcved;i++)
				{
					listqueue.enQueue(szBuffer[i]);
				}
				if(ROV_Recv_procrec(&listqueue,datapackage))//������һ��ROV״̬��
				{ // ��ȡ����
					//	MessageBox("���һ������");
					if (m_pClientEvent != NULL)
					{
						m_pClientEvent->OnRecvPacket(datapackage);
					}
					// printf("%d",Prov_stat->ATT.roll);
				}
				Send();
				//	if (dwFlag != 0x5354524301000000)
				//	{ // ����֤��־����
				//		m_byteStream.Reset();
				//		if (m_pClientEvent != NULL)
				//		{
				//			m_pClientEvent->OnError("����֤��־����");
				//		}
				//		break;
				//	}

				//	int nLength;
				//	if (!m_byteStream.Read(nLength))
				//	{ // ��ȡ����
				//		m_byteStream.Reset();
				//		if (m_pClientEvent != NULL)
				//		{
				//			m_pClientEvent->OnError("��ȡ����");
				//		}
				//		break;
				//	}
				//	if (nLength > MAX_PACKET_BODY_LEN)
				//	{ // ��ʵ�����ݵĴ�С��������
				//		m_byteStream.Reset();
				//		if (m_pClientEvent != NULL)
				//		{
				//			m_pClientEvent->OnError("��ʵ�����ݵĴ�С��������");
				//		}
				//		break;
				//	}

				//	if ((m_byteStream.GetReadOffset() + nLength) > m_byteStream.GetNumberOfBytesUsed())
				//	{ // ���岻ȫ, �ָ����������ȡ�㣬�ȴ��´δ���
				//		m_byteStream.SetReadOffset(unByteReadOffset);
				//		break;
				//	}

				//	if (m_pClientEvent != NULL)
				//	{
				//		CByteStream byteStream(m_byteStream.GetData() + m_byteStream.GetReadOffset(), nLength, false);
				//		m_pClientEvent->OnRecvPacket(byteStream);
				//	}

				//	m_byteStream.SetReadOffset(m_byteStream.GetReadOffset() + nLength);
				//}

				// �����Ѷ�ȡ���ڴ�
				//if (m_byteStream.GetReadOffset() > 0 && m_byteStream.GetNumberOfBytesUsed() >= m_byteStream.GetReadOffset())
				//{
				//	if (m_byteStream.GetNumberOfBytesUsed() == m_byteStream.GetReadOffset())
				//	{
				//		m_byteStream.Reset();
				//	}
				//	else
				//	{
						// �Ѳ��������ؿ���ǰ�湩�´ζ�
				//		CByteStream byteStream(m_byteStream.GetData() + m_byteStream.GetReadOffset(), m_byteStream.GetNumberOfBytesUsed() - m_byteStream.GetReadOffset(), true);
				//		m_byteStream.Reset();
				//		m_byteStream.Write(byteStream.GetData(), byteStream.GetNumberOfBytesUsed());
				//	}
				//}
			}
		}
	}
	catch (Poco::Net::ConnectionResetException& exc)
	{
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("�����쳣:%s"), CStringUtil::AnsiToTStr(exc.displayText()).c_str());

		this->Disconnect();
		if (m_pClientEvent != NULL)
		{
			m_pClientEvent->OnCloseConnection();
		}
		FastMutex::ScopedLock lock(m_mciMutex);
		m_bManualDisconn = false;
	}
	catch (Poco::TimeoutException& exc)
	{
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("�����쳣:%s"), CStringUtil::AnsiToTStr(exc.displayText()).c_str());		

		this->Disconnect();
		if (m_pClientEvent != NULL)
		{
			m_pClientEvent->OnCloseConnection();
		}
		FastMutex::ScopedLock lock(m_mciMutex);
		m_bManualDisconn = false;
	}
	catch (Poco::Exception& exc)
	{
		if (m_pClientEvent != NULL)
		{
			m_pClientEvent->OnError(exc.displayText());
		}
	}
}

E_NET_STATE CPacketizedTcpClient::State()
{
	FastMutex::ScopedLock lock(m_mciMutex);

	return m_netState;
}

SocketAddress CPacketizedTcpClient::GetSockAddr( void )
{
	return m_sockAddr;
}


//////////////////////////////////////////////////////////////////////////



