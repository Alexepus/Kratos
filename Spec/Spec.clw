; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHardSetupDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Spec.h"
LastPage=0

ClassCount=15
Class1=CSpecApp
Class2=CMassSpecDoc
Class3=CSpecView
Class4=CMainFrame

ResourceCount=14
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_TR_SETTINGS_DLG
Class6=CThermoSettingsDlg
Class7=CColorButton
Resource4=IDD_FF_TABLE_DLG
Class8=CCalibDlg
Resource5=IDD_INSPECT_DLG (English (U.S.))
Class9=CMeasuringOptDlg
Resource6=IDR_TOOLBAR_TR
Class10=CHardSetupDlg
Resource7=IDD_ABOUTBOX (English (U.S.))
Class11=CAppearSetupDlg
Resource8=IDD_HEATER_SETUP_DLG
Resource9=IDD_ADCDAC_DLG
Resource10=IDD_MEASURING_OPT_DLG (English (U.S.))
Class12=CHeaterSetupDlg
Resource11=IDD_CALIB_DLG (English (U.S.))
Class13=CAdcDacDlg
Resource12=IDD_APPEARANCE_SETUP_DLG
Class14=CFeedForwTableDlg
Resource13=IDD_HARDWARE_SETUP_DLG
Class15=CInspectDlg
Resource14=IDR_MAINFRAME (English (U.S.))

[CLS:CSpecApp]
Type=0
HeaderFile=Spec.h
ImplementationFile=Spec.cpp
Filter=T
LastObject=ID_FILE_NEW
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CMassSpecDoc]
Type=0
HeaderFile=MassSpecDoc.h
ImplementationFile=MassSpecDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CMassSpecDoc

[CLS:CSpecView]
Type=0
HeaderFile=SpecView.h
ImplementationFile=SpecView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_TR_SYNC_MSP


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrame.h
ImplementationFile=MainFrame.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_SETTINGS_HEATER_SETUP




[CLS:CAboutDlg]
Type=0
HeaderFile=Spec.cpp
ImplementationFile=Spec.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
CommandCount=16
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command16=ID_APP_ABOUT

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
CommandCount=14
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_APP_ABOUT
Command8=ID_BUTTON_START
Command9=ID_BUTTON_STOP
CommandCount=9

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=IDM_FILE_SAVEAS_EASYPLOT
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_TR_TRSETTINGS
Command13=ID_TR_SYNC_DXPS
Command14=ID_TR_SYNC_MSP
Command15=ID_TR_START
Command16=ID_TR_PRIME
Command17=ID_TR_WAIT
Command18=ID_TR_CONTINUE
Command19=ID_TR_MAINOFF
Command20=ID_VIEW_MASSES
Command21=ID_VIEW_MASSCHART
Command22=ID_VIEW_TEMPERATURECHART
Command23=ID_VIEW_TOOLBAR
Command24=ID_VIEW_STATUS_BAR
Command25=ID_SETTINGS_CALIBRATION
Command26=ID_SETTINGS_MEASURINGOPTIONS
Command27=ID_SETTINGS_APPEARANCE
Command28=ID_SETTINGS_HEATER_SETUP
Command29=ID_SETTINGS_HARDWARESETUP
Command30=ID_SETTINGS_ADCSDACS
Command31=ID_APP_ABOUT
CommandCount=31

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=9
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC_BUILD_DATE,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342177287
Control9=IDC_STATIC,button,1342177287

[DLG:IDD_CALIB_DLG (English (U.S.))]
Type=1
Class=CCalibDlg
ControlCount=4
Control1=IDC_BUTTON_CALIB_OK,button,1342242816
Control2=IDC_LIST_CALIB,SysListView32,1350631433
Control3=IDC_BUTTON_CALIB_CANCEL,button,1342242816
Control4=IDC_BUTTON_CALIB_INSPECT,button,1342242816

[CLS:CCalibDlg]
Type=0
HeaderFile=CalibDlg.h
ImplementationFile=CalibDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CCalibDlg
VirtualFilter=dWC

[DLG:IDD_MEASURING_OPT_DLG (English (U.S.))]
Type=1
Class=CMeasuringOptDlg
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_MASS_START,edit,1350631552
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_MASS_STOP,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_MASS_STEP,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_MASS_SETUP_DELAY,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_DATETIMEPICKER,SysDateTimePick32,1342242857
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT_CALIB_MASS_SETUP_DELAY,edit,1350631552
Control19=IDC_STATIC,static,1342308352

[CLS:CMeasuringOptDlg]
Type=0
HeaderFile=MeasuringOptDlg.h
ImplementationFile=MeasuringOptDlg.cpp
BaseClass=CDialog
Filter=W
LastObject=CMeasuringOptDlg
VirtualFilter=dWC

[DLG:IDD_HARDWARE_SETUP_DLG]
Type=1
Class=CHardSetupDlg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,button,1342177287
Control5=IDC_COMBO_INTEGRATION,combobox,1478557699
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_RANGE,combobox,1478557699
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_COM_PORT_NAME,edit,1350631552
Control11=IDC_BUTTON_COM_RECONNECT,button,1342242816

[CLS:CHardSetupDlg]
Type=0
HeaderFile=HardSetupDlg.h
ImplementationFile=HardSetupDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_COM_PORT_NAME
VirtualFilter=dWC

[DLG:IDD_APPEARANCE_SETUP_DLG]
Type=1
Class=CAppearSetupDlg
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_APPEARANCE_CALIB_CHART,ChartCtrl,1342242816
Control5=IDC_COMBO_SERIES1_TYPE,combobox,1344339971
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_SERIES1_ELEMENT,combobox,1344339971
Control9=IDC_BUTTON_SERIES1_COLOR,button,1342242827
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_SLIDER_SERIES1_SIZE,msctls_trackbar32,1342242817
Control13=IDC_STATIC_SIZE,static,1342308352
Control14=IDC_STATIC,button,1342177287
Control15=IDC_DATETIMEPICKER_KEEP,SysDateTimePick32,1342242857
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_CHECK_SHOW_POWER,button,1342242819
Control19=IDC_APPEAR_TREE,SysTreeView32,1350631479

[CLS:CAppearSetupDlg]
Type=0
HeaderFile=AppearSetupDlg.h
ImplementationFile=AppearSetupDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_SERIES1_COLOR

[CLS:CColorButton]
Type=0
HeaderFile=ColorButton.h
ImplementationFile=ColorButton.cpp
BaseClass=CButton
Filter=W
VirtualFilter=BWC

[DLG:IDD_TR_SETTINGS_DLG]
Type=1
Class=CThermoSettingsDlg
ControlCount=43
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_PRIME,edit,1350631552
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_BETA_PLUS,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_T1,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_BETA_MINUS,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_DWELL1,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_EDIT_T2,edit,1350631552
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT_DWELL2,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_CHECK_REPEATE,button,1342242851
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_HEATING_RATE,edit,1350631552
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_EDIT_COOLING_RATE,edit,1350631552
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,button,1342177287
Control33=IDC_STATIC,static,1342308352
Control34=IDC_EDIT_COLD_LEAD,edit,1350631552
Control35=IDC_STATIC,static,1342308352
Control36=IDC_EDIT_COUPLE_FILE,edit,1350633600
Control37=IDC_BUTTON_BROWSE_COUPLECALIB,button,1342242816
Control38=IDC_STATIC,static,1342308352
Control39=IDC_BUTTON_TR_SAVE,button,1342242816
Control40=IDC_BUTTON_TR_LOAD,button,1342242816
Control41=IDC_EDIT_START_DELAY,edit,1350631552
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352

[CLS:CThermoSettingsDlg]
Type=0
HeaderFile=ThermoSettingsDlg.h
ImplementationFile=ThermoSettingsDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_PRIME

[TB:IDR_TOOLBAR_TR]
Type=1
Class=?
Command1=ID_TR_START
Command2=ID_TR_PRIME
Command3=ID_TR_WAIT_CONT
Command4=ID_TR_MAINOFF
Command5=ID_TR_SYNC_DXPS
Command6=ID_TR_SYNC_MSP
CommandCount=6

[DLG:IDD_ADCDAC_DLG]
Type=1
Class=CAdcDacDlg
ControlCount=35
Control1=IDC_STATIC,button,1342177287
Control2=IDC_CHECK_MANUAL_HEATER,button,1342242819
Control3=IDC_SLIDER_HEATER,msctls_trackbar32,1476460564
Control4=IDC_CHECK_MANUAL_MSP,button,1342242819
Control5=IDC_SLIDER_MSP,msctls_trackbar32,1476460564
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC_ADC0,static,1476526080
Control10=IDC_STATIC_AVER_ADC0,static,1342308352
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC_ADC1,static,1476526080
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC_ADC2,static,1476526080
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC_ADC3,static,1476526080
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC_AVER_ADC1,static,1342308352
Control23=IDC_STATIC_AVER_ADC2,static,1342308352
Control24=IDC_STATIC_AVER_ADC3,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC_DAC0,static,1342308352
Control27=IDC_STATIC,button,1342177287
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC_DAC1,static,1342308352
Control30=IDC_STATIC,static,1476526080
Control31=IDC_STATIC_ADC6,static,1476526080
Control32=IDC_STATIC,static,1476526080
Control33=IDC_STATIC_ADC7,static,1476526080
Control34=IDC_STATIC,button,1342177287
Control35=IDC_CHECK_ADC_ON,button,1342177283

[CLS:CAdcDacDlg]
Type=0
HeaderFile=AdcDacDlg.h
ImplementationFile=AdcDacDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CHECK_ADC_ON
VirtualFilter=dWC

[DLG:IDD_FF_TABLE_DLG]
Type=1
Class=CFeedForwTableDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_FF,SysListView32,1350631425
Control4=IDC_BUTTON_LOAD_FF,button,1342242816

[CLS:CFeedForwTableDlg]
Type=0
HeaderFile=FeedForwTableDlg.h
ImplementationFile=FeedForwTableDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_LOAD_FF
VirtualFilter=dWC

[DLG:IDD_INSPECT_DLG (English (U.S.))]
Type=1
Class=CInspectDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_INSPECT_A,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_INSPECT_B,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_INSPECT_C,edit,1350631552
Control10=IDC_BUTTON_INSPECT_DEFAULTS,button,1342242816

[CLS:CInspectDlg]
Type=0
HeaderFile=InspectDlg.h
ImplementationFile=InspectDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CInspectDlg
VirtualFilter=dWC

[DLG:IDD_HEATER_SETUP_DLG]
Type=1
Class=CHeaterSetupDlg
ControlCount=44
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_KP,edit,1350631552
Control8=IDC_EDIT_KI,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_KD,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC_VP,static,1342308352
Control13=IDC_STATIC_VI,static,1342308352
Control14=IDC_STATIC_VD,static,1342308352
Control15=IDC_BUTTON_PID_COEF_LOAD,button,1342242816
Control16=IDC_STATIC_PID_OUT,static,1342308352
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_EDIT_KFF,edit,1350631552
Control21=IDC_STATIC_FEEDFORW_V,static,1342308352
Control22=IDC_STATIC,static,1342181383
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC_FEEDFORW_VF,static,1342308352
Control26=IDC_STATIC_OUTPUT,static,1342308352
Control27=IDC_STATIC_VP_F,static,1342308352
Control28=IDC_STATIC_VI_F,static,1342308352
Control29=IDC_STATIC_VD_F,static,1342308352
Control30=IDC_STATIC,button,1342177287
Control31=IDC_STATIC,static,1342308352
Control32=IDC_EDIT_DERIV_SAMPLES,edit,1350631552
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352
Control36=IDC_EDIT_VI_MAX,edit,1350631552
Control37=IDC_EDIT_ZERO_VI,edit,1350631552
Control38=IDC_BUTTON_PID_PARAM_LOAD2,button,1342242816
Control39=IDC_STATIC,static,1342181383
Control40=IDC_BUTTON_ADC_DAC,button,1342242816
Control41=IDC_STATIC,static,1342308352
Control42=IDC_EDIT_VI_DECAY,edit,1350631552
Control43=IDC_BUTTON_FF_TABLE,button,1342242816
Control44=IDC_BUTTON2,button,1342242816

[CLS:CHeaterSetupDlg]
Type=0
HeaderFile=heatersetupdlg.h
ImplementationFile=heatersetupdlg.cpp
BaseClass=CDialog
LastObject=CHeaterSetupDlg

