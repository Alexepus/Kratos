#pragma once

void SaveEasyPlotFile(FILE* fp, char* FileName);
int  SaveEasyPlotTable(FILE* fp, CRegion* pReg);
void SaveEasyPlotMC(FILE* fp, char* FileName, int DocType);
void SaveOrigin(FILE* fp, char* FileName);
void GetWord(CRegion* pReg, char* str, int nW);
void ParseXPSFile(FILE *fp);