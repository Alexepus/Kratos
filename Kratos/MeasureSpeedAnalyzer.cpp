#include "stdafx.h"
#include "MeasureSpeedAnalyzer.h"
#include "ProgNew.h"

extern CProgNewApp theApp;

MeasureSpeedAnalyzer::MeasureSpeedAnalyzer()
{
	AddPoint(static_cast<int>(std::floor(theApp.Ini.StatisticsMediumExtraDelay.Value + 0.5)));
}

MeasureSpeedAnalyzer::~MeasureSpeedAnalyzer()
{
	try
	{
		SaveToIni();
	}
	catch (...)
	{
	}
}

void MeasureSpeedAnalyzer::RegisterPointTime(int regionId, int passNumber, int pointIndex, int expectedPointMs)
{
	const int maxTimeDif = 2000; //2sec
	auto timestamp = GetTickCount();
	int timeDif = static_cast<int>(timestamp - _lastTimestamp) - expectedPointMs;
	if (regionId == _lastRegionId && passNumber == _lastPassNumber && pointIndex == _lastPointIndex + 1 && timeDif < maxTimeDif)
		AddPoint(timeDif);
	
	_lastRegionId = regionId;
	_lastPassNumber = passNumber;
	_lastPointIndex = pointIndex;
	_lastTimestamp = timestamp;
}

void MeasureSpeedAnalyzer::AddPoint(int v)
{
	_pointsAdded++;
	_statAnalyzer.AddPoint(v);
	if (_pointsAdded % 100 == 99)
		SaveToIni();
}

void MeasureSpeedAnalyzer::SaveToIni()
{
	theApp.Ini.StatisticsMediumExtraDelay.Value = GetAverage();
	theApp.Ini.StatisticsMediumExtraDelay.Write();
}
