#include "ComThreadBasic.h"
#pragma once

// Стактура данных для передачи по MailSlot "Spec_SerialIn"/"Spec_SerialOut"
struct SpecIpcdata0
{
	int CommandType;
	ComMsg comMsg;
	char StringMgs[65];
};