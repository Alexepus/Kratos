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

	// ���������� ���������� ���������� � ��
	void SetKeRetard(long retard) override;

	// ���������� HV (� �������)
	void SetHv(long voltage) override;

	// ������� �������� ��������
	long ReadCounter() override;

	// ���������� � ��������� ������. time_ms - ����� � ��
	void SetAndStartTimer(int time_ms) override;

protected:
	void CheckSettings() override;
};

