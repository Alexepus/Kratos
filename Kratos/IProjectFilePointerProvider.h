#pragma once
#define KRATOS_KEY0 0x01
#define KRATOS_KEY1 0x03
#define KRATOS_KEY (KRATOS_KEY0 << 8 + KRATOS_KEY1)
#define XPS_TYPE 0xbc
#define DXPS_TYPE 0xbd

class IProjectFilePointerProvider
{
public:
	virtual FILE* GetProjectFilePointer() = 0;
	virtual ~IProjectFilePointerProvider(){}
};