#include "ComThreadBasic.h"
#pragma once

// �������� ������ ��� �������� �� MailSlot "Spec_SerialIn"/"Spec_SerialOut"
struct SpecIpcdata0
{
	int CommandType;
	ComMsg comMsg;
	char StringMgs[65];
};