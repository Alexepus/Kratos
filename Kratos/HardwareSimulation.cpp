#include "stdafx.h"
#include "HardwareSimulation.h"
#include <random>
#include "Exceptions.h"

HardwareSimulation::HardwareSimulation() : _retard(0), _dis(0., 0.2)
{
}

void HardwareSimulation::Initialize()
{
}

void HardwareSimulation::SetKeRetard(long retard)
{
	_retard = retard;
}

void HardwareSimulation::SetHv(long hv)
{
}

long HardwareSimulation::ReadCounter()
{
	return (int)((sin(_retard*0.0002) + 1.01 + _dis(_gen)) * 1000);
}

void HardwareSimulation::SetAndStartTimer(int time_ms)
{
}

