#pragma once

#include <string>


// UDP�ͻ����¼��ӿ�
class IPacketizedUdpClientEvent
{
public:
	virtual ~IPacketizedUdpClientEvent() {}
	virtual int OnRecvPacket(unsigned char *byteStream,int datelen) = 0;
	virtual void OnCloseConnection() = 0;
	virtual void OnError(std::string strDisplayText) = 0;
};

// UDP�ͻ��˽ӿ�
class IPacketizedUdpClient
{
public:
	virtual ~IPacketizedUdpClient() {}

	virtual bool Connect(const char* szHostAddress, unsigned short unPort) = 0;
	virtual void Disconnect() = 0;
	virtual void SetClientListener(IPacketizedUdpClientEvent* pClientEvent) = 0;
	virtual bool Send() = 0;
};