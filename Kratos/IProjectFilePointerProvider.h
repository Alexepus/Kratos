#pragma once
class IProjectFilePointerProvider
{
public:
	virtual FILE* GetProjectFilePointer() = 0;
	virtual ~IProjectFilePointerProvider(){}
};