#ifndef _THREADSAFEQUEUE2_H_
#define _THREADSAFEQUEUE2_H_
#include <queue>
#include "ComThread.h"

class CThreadSafeQueue2
{
private:
	std::queue<ComMsg>				m_oQueue;	//contains the actual data
	CRITICAL_SECTION		m_csData;	//to synchroize access to m_csData among multiple threads
	//HANDLE					m_hEvent;	//for signalling presence of absence of data
public:
	//create a manual reset event initially signalled.
	//This event will be signalled and shall remain so whenever there is data in the queue and
	//it shall be reset as long as queue is empty
	CThreadSafeQueue2();
	//close the event handle
	~CThreadSafeQueue2();

	//public methods to Push data to queue
	void Push(const ComMsg& oNewData);
	//public methods to Pop data from queue
	int Pop(ComMsg* pData);
	int GetSize();
	//helper method to get the event handle
	//HANDLE GetEvent(){return m_hEvent;};
};

#endif