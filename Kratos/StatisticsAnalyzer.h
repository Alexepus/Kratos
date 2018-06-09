#pragma once
#include <deque>
#include <queue>

#undef min
#undef max

template <typename T> 
class AverageCounter
{
	const int _maxHistoryLength = 1000;
	std::queue<T> _history;
public:
	void Add(T v)
	{
		_history.push(v);
		if (StoredCount == _maxHistoryLength)
		{
			T oldVal = _history.front();
			_history.pop();
			Average = ((v - oldVal) + StoredCount * Average) / StoredCount;
		}
		else
		{
			Average = (v + StoredCount * Average) / (StoredCount + 1);
			StoredCount++;
		}
	}

	double Average = 0.0;

	int StoredCount = 0;
};


class StatisticsAnalyzer
{
	int _min = std::numeric_limits<int>::max();
	int _max = std::numeric_limits<int>::min();
	AverageCounter<int> _averageCalculator;
	AverageCounter<long long> _averageOfSquaredCalculator;
	
public:
	StatisticsAnalyzer();
	~StatisticsAnalyzer();

	double GetAverage() const;
	double GetStdDeviation() const;
	int GetMin() const;
	int GetMax() const;
	int GetStatPointsCount() const;
	void AddPoint(int v);
};

