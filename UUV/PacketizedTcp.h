#pragma once

#include <string>


// �ͻ����¼��ӿ�
class IPacketizedTcpClientEvent
{
public:
	virtual ~IPacketizedTcpClientEvent() {}
	virtual int OnGetSendPacket(unsigned char** byteStream,int* datalen)=0;
	virtual int OnRecvPacket(unsigned char *byteStream) = 0;
	virtual void OnCloseConnection() = 0;
	virtual void OnError(std::string strDisplayText) = 0;
};

// �ͻ��˽ӿ�
class IPacketizedTcpClient
{
public:
	virtual ~IPacketizedTcpClient() {}

	virtual bool Connect(const char* szHostAddress, unsigned short unPort) = 0;
	virtual void Disconnect() = 0;
	virtual void SetClientListener(IPacketizedTcpClientEvent* pClientEvent) = 0;
	virtual bool Send() = 0;
	virtual E_NET_STATE State() = 0;
};


