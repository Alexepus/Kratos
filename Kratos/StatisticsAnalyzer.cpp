#include "stdafx.h"
#include "StatisticsAnalyzer.h"
#include "ProgNew.h"

extern CProgNewApp theApp;

StatisticsAnalyzer::StatisticsAnalyzer()
{
	AddPoint(static_cast<int>(std::floor(theApp.Ini.StatisticsMediumExtraDelay.Value + 0.5)));
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
	_pointsAdded++;
	_averageCalculator.Add(v);
	_averageOfSquaredCalculator.Add(static_cast<long long>(v)*v);
	if(v > _max)
		_max = v;
	if (v < _min)
		_min = v;
	if (_pointsAdded % 100 == 99)
	{
		theApp.Ini.StatisticsMediumExtraDelay.Value = GetAverage();
		theApp.Ini.StatisticsMediumExtraDelay.Write();
	}
}
