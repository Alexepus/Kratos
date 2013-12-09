//
BOOL SaveBinaryFile(FILE* fp);
BOOL ReadBinaryFile(FILE* fp);
BOOL SaveDataToFile(FILE* fp, CRegion* pReg, int N, DATA_OUT* data);
BOOL SaveDataInToFile(FILE* fp, CRegion* pReg);
void SaveEasyPlotFile(FILE* fp, char* FileName);
int  SaveEasyPlotTable(FILE* fp, CRegion* pReg);
void SaveEasyPlotMC(FILE* fp, char* FileName, int DocType);
void SaveOrigin(FILE* fp, char* FileName);
void GetWord(CRegion* pReg, char* str, int nW);
void EmptyAllData();
void ParseXPSFile(FILE *fp);