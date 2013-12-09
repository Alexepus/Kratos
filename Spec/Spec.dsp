# Microsoft Developer Studio Project File - Name="Spec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Spec - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Spec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Spec.mak" CFG="Spec - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Spec - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Spec - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Spec - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Spec - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Spec - Win32 Release"
# Name "Spec - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AdcDacDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppearSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Approx.cpp
# End Source File
# Begin Source File

SOURCE=.\CalibDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalibThread.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartAxisLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartLegend.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartLinePointsSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartLineSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartPointsSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartTrapeziumSerie.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\ComFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\ComThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ComThreadBasic.cpp
# End Source File
# Begin Source File

SOURCE=.\FeedForwTableDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Functions.cpp
# End Source File
# Begin Source File

SOURCE=.\HardSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HeaterCommands.cpp
# End Source File
# Begin Source File

SOURCE=.\HeaterSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HeatingThread.cpp
# End Source File
# Begin Source File

SOURCE=.\iniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\IniValues.cpp
# End Source File
# Begin Source File

SOURCE=.\InspectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogToFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MassCalibDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MassSpecDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasuringOptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasuringThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgPool.cpp
# End Source File
# Begin Source File

SOURCE=.\Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Spec.cpp
# End Source File
# Begin Source File

SOURCE=.\Spec.rc
# End Source File
# Begin Source File

SOURCE=.\SpecView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ThermoHistDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ThermoReg.cpp
# End Source File
# Begin Source File

SOURCE=.\ThermoSettingsDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AdcDacDlg.h
# End Source File
# Begin Source File

SOURCE=.\AppearSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\Approx.h
# End Source File
# Begin Source File

SOURCE=.\CalibDlg.h
# End Source File
# Begin Source File

SOURCE=.\CamacUnits.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartAxis.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartAxisLabel.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartCtrl.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartCursor.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartGrid.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartLegend.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartLinePointsSerie.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartLineSerie.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartObject.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartPointsSerie.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartSerie.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartTitle.h
# End Source File
# Begin Source File

SOURCE=.\HSChart\ChartTrapeziumSerie.h
# End Source File
# Begin Source File

SOURCE=.\ColourPicker.h
# End Source File
# Begin Source File

SOURCE=.\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\ComFunctions.h
# End Source File
# Begin Source File

SOURCE=.\ComThread.h
# End Source File
# Begin Source File

SOURCE=.\ComThreadBasic.h
# End Source File
# Begin Source File

SOURCE=.\FeedForwTableDlg.h
# End Source File
# Begin Source File

SOURCE=.\Functions.h
# End Source File
# Begin Source File

SOURCE=.\HardSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\HeaterCommands.h
# End Source File
# Begin Source File

SOURCE=.\HeaterSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\iniFile.h
# End Source File
# Begin Source File

SOURCE=.\InspectDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogToFile.h
# End Source File
# Begin Source File

SOURCE=.\MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\MassCalibDoc.h
# End Source File
# Begin Source File

SOURCE=.\MassSpecDoc.h
# End Source File
# Begin Source File

SOURCE=.\MeasuringOptDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsgPool.h
# End Source File
# Begin Source File

SOURCE=.\Properties.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Spec.h
# End Source File
# Begin Source File

SOURCE=.\SpecView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ThermoHistDoc.h
# End Source File
# Begin Source File

SOURCE=.\ThermoReg.h
# End Source File
# Begin Source File

SOURCE=.\ThermoSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Threads.h
# End Source File
# Begin Source File

SOURCE=.\ThreadSafeQueue.h
# End Source File
# Begin Source File

SOURCE=.\VCMCDM.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor_g.cur
# End Source File
# Begin Source File

SOURCE=.\res\divsize.cur
# End Source File
# Begin Source File

SOURCE=.\res\MassSpecDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Spec.ico
# End Source File
# Begin Source File

SOURCE=.\res\Spec.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Spectrum.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\VCMCDM.LIB
# End Source File
# End Group
# Begin Source File

SOURCE=.\Spec.reg
# End Source File
# End Target
# End Project
