// IPS6.h/IPS6.cpp реализуют интерфейс, подобный VCMCD(M).H за единственным исключением: 
// драйвер надо открывать и закрывать 
#include "sli6_ext.h"

int            Camac_Open(int c);
void           Camac_Close(int c);
void           Camac_CloseAll();

long           Camac_ReadLong(int c, int n, int a, int f);
void           Camac_WriteLong(int c, int n, int a, int f, long d);
unsigned short Camac_ReadWord(int c, int n, int a, int f);
void           Camac_WriteWord(int c, int n, int a, int f, unsigned short d);
void           Camac_Reset(int c);

unsigned short Camac_Version(void);
int            Camac_x(void);
int            Camac_q(void);
char *         Camac_ErrorMessage(void);
int            Camac_ErrorCode(void);
void           Camac_EnableMessage(int State);
