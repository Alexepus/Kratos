#define OS_Lib_Type OS_cin
#define OS_USE_CSEM_I

#define OS_Timer_Tick_Set() { TMR2IF = 0; }
#define OS_Is_Timer() TMR2IF
#define OS_TMR_TICK 249
