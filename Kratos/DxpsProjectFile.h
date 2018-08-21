#pragma once
#include "IProjectFilePointerProvider.h"

class DxpsProjectFile
{
	IProjectFilePointerProvider* _projectFilePointerProvider;
public:
	DxpsProjectFile(IProjectFilePointerProvider* projectFilePointerProvider);
	~DxpsProjectFile();

	void ReadProject(FILE* fp, int FileVersion);
	void SaveProject(FILE* fp);
};

