#include "stdafx.h"
#include "CamacHardware.h"
#include "hardware.h"
#include "ProgNew.h"

extern CProgNewApp theApp;

CamacHardware::CamacHardware()
{
	_crateN = theApp.Ini.CrateNumber.Value;
	_retardCalibration = theApp.Ini.RetardCalibration.Value;
}

void CamacHardware::Initialize()
{
	CheckSettings();
	Camac_EnableMessage(FALSE); //camac driver won't display MessageBoxes
	Camac_Open(_crateN);
}

inline CamacHardware::~CamacHardware()
{
	Camac_Close(_crateN);
}
