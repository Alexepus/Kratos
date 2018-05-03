#pragma once
#include "IHardware.h"

class CamacHardware : public IHardware
{
protected:
	int _crateN;
	double* _retardCalibration;

public:
	CamacHardware();

	// Инициализировать модуль
	void Initialize() override;

	~CamacHardware() override;

protected:
	virtual void CheckSettings() = 0;
};

