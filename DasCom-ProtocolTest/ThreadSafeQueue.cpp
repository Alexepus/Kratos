#include "stdafx.h"
#include "ThreadSafeQueue.h"


template <class T> CThreadSafeQueue<T>::CThreadSafeQueue()
	{ 
		InitializeCriticalSection(&m_csData);
		//m_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	};
	//close the event handle

template <class T> CThreadSafeQueue<T>::~CThreadSafeQueue()
	{ 
		DeleteCriticalSection(&m_csData);
		//CloseHandle(m_hEvent);
	};

	//public methods to Push data to queue

template <class T> void CThreadSafeQueue<T>::Push(T& oNewData)
	{
		EnterCriticalSection(&m_csData);
		//push new element to queue
		m_oQueue.push(oNewData);
		//now that there is atleast one element, set the event
		//SetEvent(m_hEvent);
		LeaveCriticalSection(&m_csData);
	};
	//public methods to Pop data from queue

template <class T> int CThreadSafeQueue<T>::Pop(T* pData)
	{
		EnterCriticalSection(&m_csData);
		//first get the topmost data block
		if(m_oQueue.size())
		{
			*pData=m_oQueue.front();
			//memcpy(pData,m_oQueue.front(), sizeof(T));
			//next remove it from queue
			m_oQueue.pop();
			//now, check for new size.. if no more elements in queue
			//reset the event
			//if(!m_oQueue.size())
				//ResetEvent(m_hEvent);

			LeaveCriticalSection(&m_csData);
			return TRUE;
		}
		else
		{
			//ResetEvent(m_hEvent);
			LeaveCriticalSection(&m_csData);
			return FALSE;
		}
	};

	//helper method to get the event handle
	//HANDLE GetEvent(){return m_hEvent;}
