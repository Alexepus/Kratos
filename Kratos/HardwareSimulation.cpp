#include "stdafx.h"
#include "HardwareSimulation.h"

void HardwareSimulation::Initialize()
{
}

void HardwareSimulation::SetKeRetard(long retard)
{
}

void HardwareSimulation::SetHv(long retard)
{
}

long HardwareSimulation::ReadCounter()
{
	static int count = 0;
	return (int)((pow(sin(count*0.1), 2) + 0.5) * 10);
}

void HardwareSimulation::SetAndStartTimer(int time_ms)
{
}

