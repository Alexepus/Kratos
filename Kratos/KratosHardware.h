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

	// ���������������� ������
	void Initialize() override;

	// ���������� ���������� ���������� � ��
	void SetKeRetard(long retard) override;

	// ���������� HV (� �������)
	void SetHv(long retard) override;

	// ������� �������� ��������
	long ReadCounter() override;

	// ���������� � ��������� ������. time_ms - ����� � ��
	void SetAndStartTimer(int time_ms) override;

protected:
	void CheckSettings() override;
};

