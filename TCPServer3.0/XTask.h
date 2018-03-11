#ifndef __XTASK_H__
#define __XTASK_H__

#include <mutex>
#include <list>
#include <chrono>
#include <thread>

class XTask
{
public:
	XTask();
	virtual ~XTask();

	virtual void DoTask();
};

class XTaskServer
{
private:
	std::list<XTask*> _Tasks;					//任务列表
	std::list<XTask*> _TasksCache;				//任务缓冲区
	std::mutex _TaskSCacheMutex;				//任务缓冲区锁

	void OnRun();

public:
	void Start();
	void AddTask(XTask* pTask);
};

#endif // 
