#pragma once
class DxpsProjectFile
{
	FILE* _fpPrj = nullptr;
public:
	DxpsProjectFile();
	~DxpsProjectFile();

	void ReadProject(FILE* fp, int FileVersion);
};

