#ifndef __MYCLIENT_H__
#define __MYCLIENT_H__

#include "../XSrc/XTCPClient.h"
#include "../XSrc/XRecvByteStream.h"

class MyClient : public XTCPClient
{
public:
	void DoMsg(MsgHeader* pMsgHeader);

private:
	virtual void OnRunLoopBegin();
	virtual void OnClientJoin(std::shared_ptr<XClient> pClient);
	virtual void OnClientLeave(std::shared_ptr<XClient> pClient);
	virtual void OnNetRecv(std::shared_ptr<XClient> pClient);
	virtual void OnNetSend(std::shared_ptr<XClient> pClient);
	virtual void OnNetMsgRecv(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader);
	virtual void OnNetMsgDone(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader);

	virtual void AddTask(std::function<void()> pTask);
};

#endif