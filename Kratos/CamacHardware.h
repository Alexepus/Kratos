#pragma once
#include "IHardware.h"

class CamacHardware : public IHardware
{
protected:
	int _crateN;
	int _retardCalibration;

public:
	CamacHardware();

	// Инициализировать модуль
	void Initialize() override;

	~CamacHardware();

protected:
	virtual void CheckSettings() = 0;
};

