void Pid(void);
INT16Q4 CalcTByV(INT16Q4 Voltage);
void FillTableT(void);
void FillTableInvDV(void);
INT16Q4 CalcFF(INT16Q4 TRef);

#define Q4(A) ((int)(A*16.))
#define Q8(A) ((int)(A*256.))
#define Q16(A) ((int)(A*256.*256.))
#define fl4(A) (A*0.0625)

#define MulIShift8(A, B, Y)\
{\
	MulVarA=A;\
	MulVarB=(MulVarA*B);\
	*((char*)&Y)=*(((char*)&MulVarB)+1);\
	*(((char*)&Y)+1)=*(((char*)&MulVarB)+2);\
}
#define MulIShift8Res32(A, B, Y)\
{\
	MulVarA=A;\
	Y=(MulVarA*B)>>8;\
}

#define MulIShift16(A, B, Y)\
{\
	MulVarA=A;\
	MulVarB=(MulVarA*B);\
	*((char*)&Y)=*(((char*)&MulVarB)+2);\
	*(((char*)&Y)+1)=*(((char*)&MulVarB)+3);\
}

#define Div32By16To16(A, B, Y)\
{\
}

extern long MulVarA, MulVarB; //Временные переменные при умножении
extern INT16Q8 Kp, Ki, Kd, Kff; //Коэффициенты при значениях
extern INT16Q4 Vp, Vi, Vd, Vff, Vout; //Пропорц., интегр. и диф. компоненты
extern INT16Q4 ViMax, ViMin, ViOff; //Насыщение интегральной компоненты
extern INT16Q4 TRef, TMeas; //Опорная и измеренная температура
extern INT16Q4 PidOut; //Выходное значение регулятора
extern INT16Q8 TableV[32];
extern INT16Q4 TableT[32];
extern INT16Q8 TableInvDV[32];
extern INT16Q4 FFTableTemp[4];
extern INT16Q4 FFTableOut[4];
extern INT16Q4 FFOut;
extern BYTE TableDT;
extern bit TableVLoaded,TdTLoaded; 
extern INT16Q4 TempHistFifo[16];
extern BYTE TempHistFifoIndex;
extern BYTE DifShift;
extern BYTE IDec;
extern INT16Q4 TEnd;
extern INT32Q20 DeltaT, TAccumulator;
extern long VoutAcc;
extern bit ManualHeaterControl;
extern bit FlagWait, FlagMainOff,FlagRefTempAchived;

