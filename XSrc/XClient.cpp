﻿#include "XClient.h"

XClient::XClient(SOCKET client, XIGlobalEvent* pGlobalObj, XIServerEvent* pServerObj)
	:
	_Socket(client),
	_pGlobalObj(pGlobalObj),
	_pServerObj(pServerObj),
	_RecvBuffer(_RECV_BUFFER_SIZE_),
	_SendBuffer(_SEND_BUFFER_SIZE_),
	_HeartTime(0),
	_SendTime(0)
{
}

XClient::~XClient()
{
#ifdef _WIN32
	shutdown(_Socket, SD_BOTH);
	closesocket(_Socket);
#else
	shutdown(_Socket, SHUT_RDWR);
	close(_Socket);
#endif
}

int XClient::RecvData()
{
	int nRet = _RecvBuffer.Recv(_Socket);
	if (nRet != 0)
		return  nRet;

	if (_pGlobalObj)
		_pGlobalObj->OnNetRecv(shared_from_this());

	while (_RecvBuffer.HasMsg())
	{
		if (_pGlobalObj)
			_pGlobalObj->OnNetMsgRecv(shared_from_this(), _RecvBuffer.Front());

		_RecvBuffer.Pop();
	}

	return nRet;
}

int XClient::SendData(MsgHeader* pHeader)
{
	int nRet = _SendBuffer.Push(pHeader);
	if (nRet != 0)
		return nRet;

	if (_pGlobalObj)
		_pGlobalObj->OnNetMsgDone(shared_from_this(), pHeader);

	return nRet;
}

int XClient::SendStream(XByteStream* pByteStream)
{
	return SendData((MsgHeader*)pByteStream->GetBuffer());
}

int XClient::SendData()
{
	int nRet = _SendBuffer.Send(_Socket);
	if (nRet != 0)
		return nRet;

	//发送了数据
	if (_pGlobalObj)
		_pGlobalObj->OnNetSend(shared_from_this());

	ResetSendTime();

	return 0;
}

void XClient::ResetHeartTime()
{
	_HeartTime = 0;
}

bool XClient::CheckHeartTime(time_t t)
{
	_HeartTime += t;
	if (_HeartTime >= _XCLIENT_HEART_TIME_)
	{
		//XInfo("CheckHeartTime : Client<socket=%d> timeout on time = %d! \n", (int)_Socket, (int)_HeartTime);
		return true;
	}

	return false;
}

void XClient::ResetSendTime()
{
	_SendTime = 0;
}

void XClient::CheckSendTime(time_t t)
{
	_SendTime += t;
	if (_SendTime >= _XCLIENT_SEND_TIME_)
	{
		//XInfo("CheckSendTime! \n");
		SendData();
	}
}

XIGlobalEvent* XClient::GetGlobalObj()
{
	return _pGlobalObj;
}

XIServerEvent* XClient::GetServerObj()
{
	return _pServerObj;
}

std::shared_ptr<XClient> XClient::GetSharedPrt()
{
	return shared_from_this();
}