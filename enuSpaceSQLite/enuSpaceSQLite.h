// enuSpaceSQLite.h : main header file for the enuSpaceSQLite DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CenuSpaceSQLiteApp
// See enuSpaceSQLite.cpp for the implementation of this class
//

class CenuSpaceSQLiteApp : public CWinApp
{
public:
	CenuSpaceSQLiteApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////////////
// Description : enuSpace - plugin (SQLite)
//               This plugin library is made in Expansion & Universal Cooperation.  
//               This core library used SQLite project(https://www.sqlite.org/). 
// Start date  : 2017.05
// homepage    : http://www.enu-tech.co.kr
// ENU Corporation, �̿����ֽ�ȸ��, ENU Co., Ltd
////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <map>

/////////////////////////////////////////////////////////////////////////////////
#define DEF_UNKNOWN								-1
#define DEF_INT									0
#define DEF_FLOAT								1
#define DEF_DOUBLE								2
#define DEF_BOOL								3
#define DEF_STRING								4
#define DEF_STRUCT								5
#define DEF_VARIABLE							6
#define DEF_OBJECT								7

#define TASK_TYPE_UNKNOWN						0			// [v3.5] mod : Task�� Ÿ�� ���� (�˼����� Ÿ��)
#define TASK_TYPE_PROCESS						1			// [v3.5] mod : Task�� Ÿ�� ����	(����ó���� Ÿ��)
#define TASK_TYPE_FLOW_COMPONENT_TOTAL			2			// [v3.5] mod : Task�� Ÿ�� ����	(FLOW ������Ʈ ��ü ���� Ÿ��)
#define TASK_TYPE_FLOW_COMPONENT_PAGE			3			// [v3.5] add : Task�� Ÿ�� ����	(FLOW������Ʈ ������ ���� Ÿ��)
#define TASK_TYPE_FUNCTION_COMPONENT			4			// [v3.5] add : Task�� Ÿ�� ���� (�Լ��� ������Ʈ Ÿ��)

#define DEF_NAME_LEN							64
#define DEF_MAXTEXT_LEN							2048
#define DEF_LABELTEXT_LEN						1024

struct arrayInfo
{
	int size;
	wchar_t dimension[DEF_LABELTEXT_LEN];

	public:arrayInfo()
	{
		size = 0;
		wcscpy_s(dimension, L"");
	}
};

struct VariableStruct
{
	wchar_t name[DEF_NAME_LEN];
	int     type;
	void*   pValue;
	wchar_t strValue[DEF_MAXTEXT_LEN];
	arrayInfo array;

	public:VariableStruct()
	{
		wcscpy_s(name, L"");			// [ver.56] add : �ʱ�ȭ �߰�.
		type = DEF_UNKNOWN;
		pValue = NULL;
		wcscpy_s(strValue, L"N/A");
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct FuncParam
{
	std::map<std::string, VariableStruct> InputMap;
	std::map<std::string, VariableStruct> OutputMap;

	void addParam(std::string PinName)
	{
		VariableStruct Param;
		std::wstring wide_string = std::wstring(PinName.begin(), PinName.end());
		wcscpy_s(Param.name, wide_string.c_str());
		InputMap.insert(std::pair<std::string, VariableStruct>(PinName, Param));
	}

	const VariableStruct* GetParam(std::string PinName)
	{
		const VariableStruct* pVarStruct = NULL;
		const std::map<std::string, VariableStruct>::const_iterator aLookup = InputMap.find(PinName);
		const bool bExists = aLookup != InputMap.end();
		if (bExists)
		{
			pVarStruct = &aLookup->second;
			return pVarStruct;
		}
		return pVarStruct;
	}

	void addOutput(std::string PinName)
	{
		VariableStruct Param;
		std::wstring wide_string = std::wstring(PinName.begin(), PinName.end());
		wcscpy_s(Param.name, wide_string.c_str());
		OutputMap.insert(std::pair<std::string, VariableStruct>(PinName, Param));
	}

	const VariableStruct* GetOutput(std::string PinName)
	{
		const VariableStruct* pVarStruct = NULL;
		const std::map<std::string, VariableStruct>::const_iterator aLookup = OutputMap.find(PinName);
		const bool bExists = aLookup != OutputMap.end();
		if (bExists)
		{
			pVarStruct = &aLookup->second;
			return pVarStruct;
		}
		return pVarStruct;
	}

	~FuncParam()
	{
		InputMap.clear();
		OutputMap.clear();
	}
};

typedef void(*FuncPtr)(FuncParam*);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InterfaceDataMapClear();
void SetArrayValue(std::string strVariable, void* pSrc, int iType, int iSize);
void SetReShapeArrayValue(std::string strVariable, void* pSrc, int iType, int iSize);		// enuSpace�� �޸� ����� �ٸ����, enuSpace�� �޸𸮸� ���Ҵ��� �� �����ϴ� �Լ�
double GetArrayValue(std::string strVariable);
void SetValue(std::string strVariable, double fValue);
double GetValue(std::string strVariable);
void PrintMessage(std::string strMessage);
int GetArrayIndexFromDimension(CString strOrgDim, CString strDimension);