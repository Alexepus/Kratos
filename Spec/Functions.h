#ifndef _FUNCTIONS_H_INCLUDED_
#define _FUNCTIONS_H_INCLUDED_

#define TF_OK 0
#define TF_INVALID_BETAS 1
#define TF_INVALID_TOTAL_PERIOD 2
#define TF_INVALID_COOLING_HEATING_RATE 3
#define TF_ERROR_T1_LW_T2 4

int VerifyThermoFunction(CThermoFunc *pTF);
void ReportThermoFunctionError(int ErrCode, CWnd *pWnd);
double GetTimeDouble();
double Mod(double Divident, double Divisor);
bool VerifyGUIObject(CWnd *pWnd);
#define Int16Q8High(Val) ((BYTE)(((short)((Val)*256))>>8))
#define Int16Q8Low(Val) ((BYTE)(((short)((Val)*256))&0xff))
#define Int16Q4High(Val) ((BYTE)(((short)((Val)*16))>>8))
#define Int16Q4Low(Val) ((BYTE)(((short)((Val)*16))&0xff))
#define Int16High(Val) ((BYTE)(((short)(Val))>>8))
#define Int16Low(Val) ((BYTE)(((short)(Val))&0xff))
#define Int16Q8ToDouble(ValH,ValL) ((double)((ValH)+(ValL)/256.))
#define Int16Q4ToDouble(ValH,ValL) ((double)((ValH)+(ValL)/16.))
#define BytesToShort(ValL,ValH) (short)(((short)(ValH)<<8)|(ValL))

#define CorrectInt16Q4(Val)  (((short)((Val)*16))/16.)
#define CorrectInt16Q8(Val)  (((short)((Val)*256))/256.)
#define CorrectInt16Q16(Val)  (((short)((Val)*256*256))/256./256.)
#define MinMax(Val,Min,Max)  (min(max((Val),(Min)),(Max)))
CString PrintfFTruncate(double Val, int Precision);
double Round( double Val, double RoundBy);


#endif