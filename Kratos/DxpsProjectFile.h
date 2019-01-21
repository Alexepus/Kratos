#pragma once
#include "IProjectFilePointerProvider.h"
#include "DxpsRegion.h"

class DxpsProjectFile
{
	IProjectFilePointerProvider* _projectFilePointerProvider;
public:
	DxpsProjectFile(IProjectFilePointerProvider* projectFilePointerProvider);
	~DxpsProjectFile();

	void ReadProject(FILE* fp, int FileVersion);
	void SaveProject(FILE* fp);

	BOOL WriteDxpsRegionsParam();
	BOOL WriteDxpsPoints(DxpsOutList::iterator iter);
	BOOL WriteDxpsPoints();
};

