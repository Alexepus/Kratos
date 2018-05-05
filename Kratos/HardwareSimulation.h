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
	// ���������������� ������
	void Initialize() override;

	// ���������� ���������� ���������� � ��
	void SetKeRetard(long retard) override;

	// ���������� HV (� �������)
	void SetHv(long hv) override;

	// ������� �������� ��������
	long ReadCounter() override;

	// ���������� � ��������� ������. time_ms - ����� � ��
	void SetAndStartTimer(int time_ms) override;

};

