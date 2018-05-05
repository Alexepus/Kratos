#pragma once
#include "IHardware.h"
#include <random>

class HardwareSimulation : 	public IHardware
{
	long _retard;
	std::mt19937 _gen;
	std::uniform_real_distribution<> _dis;
public:
	HardwareSimulation();
	// Инициализировать модуль
	void Initialize() override;

	// Установить тормозящее напряжение в мВ
	void SetKeRetard(long retard) override;

	// Установить HV (в вольтах)
	void SetHv(long hv) override;

	// Считать значение счетчика
	long ReadCounter() override;

	// Установить и запустить таймер. time_ms - время в мс
	void SetAndStartTimer(int time_ms) override;

};

