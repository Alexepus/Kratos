#pragma once
#include "IHardware.h"
class HardwareSimulation : 	public IHardware
{
public:
	// Инициализировать модуль
	void Initialize() override;

	// Установить тормозящее напряжение в мВ
	void SetKeRetard(long retard) override;

	// Установить HV (в вольтах)
	void SetHv(long retard) override;

	// Считать значение счетчика
	long ReadCounter() override;

	// Установить и запустить таймер. time_ms - время в мс
	void SetAndStartTimer(int time_ms) override;
};

