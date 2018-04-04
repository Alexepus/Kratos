#pragma once
#include "CamacHardware.h"

class HpHardware : 	public CamacHardware
{
	int _registerRetardN;
	int _registerHVN;
	int _counterN;
	int _timerN;
public:
	HpHardware();

	// Установить тормозящее напряжение в мВ
	void SetKeRetard(long retard) override;

	// Установить HV (в вольтах)
	void SetHv(long voltage) override;

	// Считать значение счетчика
	long ReadCounter() override;

	// Установить и запустить таймер. time_ms - время в мс
	void SetAndStartTimer(int time_ms) override;

protected:
	void CheckSettings() override;
};

