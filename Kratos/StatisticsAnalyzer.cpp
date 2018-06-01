#include "stdafx.h"
#include "StatisticsAnalyzer.h"

StatisticsAnalyzer::StatisticsAnalyzer()
{
}

StatisticsAnalyzer::~StatisticsAnalyzer()
{
}

double StatisticsAnalyzer::GetAverage() const
{
	return _averageCalculator.Average;
}

double StatisticsAnalyzer::GetStdDeviation() const
{
	return sqrt(_averageOfSquaredCalculator.Average - _averageCalculator.Average*_averageCalculator.Average);
}

int StatisticsAnalyzer::GetMin() const
{
	return _min;
}

int StatisticsAnalyzer::GetMax() const
{
	return _max;
}

void StatisticsAnalyzer::AddPoint(int v)
{
	_averageCalculator.Add(v);
	_averageOfSquaredCalculator.Add(static_cast<long long>(v)*v);
	if(v > _max)
		_max = v;
	if (v < _min)
		_min = v;
}
