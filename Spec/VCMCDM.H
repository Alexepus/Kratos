
#ifdef __cplusplus
    extern "C" {
#endif

long           __stdcall Camac_ReadLong(int c, int n, int a, int f);
void           __stdcall Camac_WriteLong(int c, int n, int a, int f, long d);
unsigned short __stdcall Camac_ReadWord(int c, int n, int a, int f);
void           __stdcall Camac_WriteWord(int c, int n, int a, int f, unsigned short d);
void           __stdcall Camac_Reset(int c);

unsigned short __stdcall Camac_Version(void);
int            __stdcall Camac_x(void);
int            __stdcall Camac_q(void);
char *         __stdcall Camac_ErrorMessage(void);
int            __stdcall Camac_ErrorCode(void);
void           __stdcall Camac_EnableMessage(int State);

#ifdef __cplusplus
    }
#endif
