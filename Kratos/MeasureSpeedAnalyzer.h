#pragma once
#include "StatisticsAnalyzer.h"

class MeasureSpeedAnalyzer
{
	int _pointsAdded = -1;
	int _lastRegionId = -1;
	int _lastPassNumber = -1;
	int _lastPointIndex = -1;
	DWORD _lastTimestamp = 0;

	StatisticsAnalyzer _statAnalyzer;
public:
	MeasureSpeedAnalyzer();
	~MeasureSpeedAnalyzer();

	double GetAverage() const 
		{ return _statAnalyzer.GetAverage(); };
	double GetStdDeviation() const 
		{return _statAnalyzer.GetStdDeviation();};
	int GetMin() const
		{return _statAnalyzer.GetMin();}
	int GetMax() const
		{return _statAnalyzer.GetMax();}
	int GetStatPointsCount() const
		{return _statAnalyzer.GetStatPointsCount();}
	void RegisterPointTime(int regionId, int passNumber, int pointIndex, int expectedPointMs);

private:
	void AddPoint(int v);
	void SaveToIni();
};

