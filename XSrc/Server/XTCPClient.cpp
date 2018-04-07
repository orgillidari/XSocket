#include "XTCPClient.h"

XTCPClient::XTCPClient()
	:
	_Client(nullptr)
{
}

XTCPClient::~XTCPClient()
{

}

void XTCPClient::Open()
{
	XNet::Go();

	assert(_Client == nullptr);

	if (_Client == nullptr)
	{
		SOCKET _Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (SOCKET_ERROR == _Socket)
		{
			printf("Error:socket!\n");
		}

		_Client = new XClient(_Socket, this, this);
	}
}

void XTCPClient::Connect(const char* ip, unsigned short port)
{
	assert(_Client != nullptr);

	sockaddr_in sinServer = {};
	sinServer.sin_family = AF_INET;
#ifdef _WIN32
	sinServer.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	sinServer.sin_addr.s_addr = inet_addr(ip);
#endif // _WIN32
	sinServer.sin_port = htons(port);
	int sinLen = sizeof(sockaddr_in);
	if (SOCKET_ERROR == connect(_Client->GetSocket(), (sockaddr*)&sinServer, sinLen))
	{
		printf("Error:connect!\n");
	}
}

void XTCPClient::Close()
{
	assert(_Client != nullptr);

	if (_Client != nullptr)
	{
		delete _Client;
		_Client = nullptr;
	}
}

bool XTCPClient::IsRun()
{
	return _Client != nullptr;
}

void XTCPClient::OnRun()
{
	if (IsRun())
	{
		SOCKET _Socket = _Client->GetSocket();

		fd_set fdRead;
		fd_set fdWrite;
		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_SET(_Socket, &fdRead);
		FD_SET(_Socket, &fdWrite);

		timeval tv = { 0, 1 };
		int ret = select((int)_Socket + 1, &fdRead, &fdWrite, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:select!\n");
			Close();
			return;
		}
		else if ( 0 == ret )
		{
			return;
		}

		if (FD_ISSET(_Socket, &fdRead))
		{
			int ret = _Client->RecvData();
			if (ret < 0)
			{
				Close();
			}
		}

		if (FD_ISSET(_Socket, &fdWrite))
		{
			int ret = _Client->SendData();
			if (ret < 0)
			{
				Close();
			}
		}
	}
}

int XTCPClient::SendData(MsgHeader* pHeader)
{
	return _Client->SendData(pHeader);
}