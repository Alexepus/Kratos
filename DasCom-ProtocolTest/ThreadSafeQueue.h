#ifndef _THREADSAFEQUEUE_H_
#define _THREADSAFEQUEUE_H_
#include <queue>


template <class T> class CThreadSafeQueue
{
private:
	std::queue<T>				m_oQueue;	//contains the actual data
	CRITICAL_SECTION		m_csData;	//to synchroize access to m_csData among multiple threads
	//HANDLE					m_hEvent;	//for signalling presence of absence of data
public:
	//create a manual reset event initially signalled.
	//This event will be signalled and shall remain so whenever there is data in the queue and
	//it shall be reset as long as queue is empty
	CThreadSafeQueue();
	//close the event handle
	~CThreadSafeQueue();

	//public methods to Push data to queue
	void Push(T& oNewData);
	//public methods to Pop data from queue
	int Pop(T* pData);

	//helper method to get the event handle
	//HANDLE GetEvent(){return m_hEvent;};
};

#endif