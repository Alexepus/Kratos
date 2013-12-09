#pragma once

class DomainLock
{
	CCriticalSection &_cs;
public:
	DomainLock(CCriticalSection &cs) : _cs(cs)
	{
		_cs.Lock();
	}
	~DomainLock()
	{
		_cs.Unlock();
	}
};