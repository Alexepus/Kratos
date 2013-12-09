#pragma once
#include <queue>
#include <Afxmt.h>

//Определение функций внутри класса связано с известным багом линковщика "error LNK2019"
template <class T>
class CThreadSafeQueue 
	{
		CCriticalSection m_CritSect;
	public:
		std::queue<T> Queue;
		HANDLE m_EventHasData;
		CThreadSafeQueue(){m_EventHasData=CreateEvent(NULL, TRUE, FALSE, NULL);}
		~CThreadSafeQueue(){CloseHandle(m_EventHasData);}
		//Внимание: только следующие 3 функции - Thread Safe
		void Push(const T& Val)
			{
			m_CritSect.Lock();
			//std::queue<T>::push(Val);
			Queue.push(Val);
			SetEvent(m_EventHasData);
			m_CritSect.Unlock();
			}
		T front_pop()
			{
			m_CritSect.Lock();
			T Val;
			if(Queue.size()!=0)
				{
				Val=Queue.front();
				Queue.pop();
				}
			if(Queue.size()==0)
				ResetEvent(m_EventHasData);
			m_CritSect.Unlock();
			return Val;
			}
		//Пробует получить данные из очереди. Возвращает true, если успешно
		bool try_front_pop(T& Val)
			{
			m_CritSect.Lock();
			bool Success=false;
			if(Queue.size()!=0)
				{
				Val=Queue.front();
				Queue.pop();
				Success=true;
				}
			if(Queue.size()==0)
				ResetEvent(m_EventHasData);
			m_CritSect.Unlock();
			return Success;
			}
		bool IsEmpty()
		{
			m_CritSect.Lock();
			bool Empty=false;
			if(Queue.empty())
				Empty=true;
			m_CritSect.Unlock();
			return Empty;
		}
	};
