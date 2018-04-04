#pragma once
#include "IHardware.h"
class HardwareSimulation : 	public IHardware
{
public:
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
};

