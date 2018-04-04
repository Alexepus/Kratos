#pragma once

class IHardware
{
public:
	// Инициализировать модуль
	virtual void Initialize() = 0;

	// Установить тормозящее напряжение в мВ
	virtual void SetKeRetard(long retard) = 0;

	// Установить HV (в вольтах)
	virtual void SetHv(long voltage) = 0;

	// Считать значение счетчика
	virtual long ReadCounter() = 0;

	// Установить и запустить таймер. time_ms - время в мс
	virtual void SetAndStartTimer(int time_ms) = 0;

	virtual ~IHardware() = default;
};

