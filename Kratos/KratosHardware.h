#pragma once
#include "CamacHardware.h"
#include "SerialCounter/SerialCounterUnit.h"

class KratosHardware : public CamacHardware
{
	int _registerN;
	SerialCounterUnit _counterUnit;
	int& _lastCounterTime;
public:
	KratosHardware();

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

protected:
	void CheckSettings() override;
};

