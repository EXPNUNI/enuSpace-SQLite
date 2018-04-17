// enuSpaceSQLite.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "enuSpaceSQLite.h"

#include "GlobalHeader.h"
#include "utility_functions.h"
#include "enuSpaceSQLiteTask.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CenuSpaceSQLiteApp

BEGIN_MESSAGE_MAP(CenuSpaceSQLiteApp, CWinApp)
END_MESSAGE_MAP()


// CenuSpaceSQLiteApp construction

CenuSpaceSQLiteApp::CenuSpaceSQLiteApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CenuSpaceSQLiteApp object

CenuSpaceSQLiteApp theApp;


// CenuSpaceSQLiteApp initialization


BOOL CenuSpaceSQLiteApp::InitInstance()
{
	CWinApp::InitInstance();

	HINSTANCE hInstance = AfxGetInstanceHandle();
	wchar_t szPath[MAX_PATH];
	GetModuleFileName(hInstance, szPath, MAX_PATH);

	wchar_t drive[MAX_PATH];               // 드라이브 명
	wchar_t dir[MAX_PATH];                 // 디렉토리 경로
	wchar_t fname[MAX_PATH];			   // 파일명
	wchar_t ext[MAX_PATH];                 // 확장자 명

	_wsplitpath_s(szPath, drive, dir, fname, ext);

	std::string strMessage;
	std::string strTenMessage = string_format("SQLite -> %s", strMessage.c_str());

	CString strDllPath;
	strDllPath.Format(L"%s%s", drive, dir);
	g_strDllPath = CStringToString(strDllPath);

	return TRUE;
}

void(*g_fcbSetValue)(wchar_t*, double) = NULL;
VariableStruct(*g_fcbGetValue)(wchar_t*) = NULL;
void(*g_fcbSetArrayValue)(wchar_t*, void*, int, int) = NULL;
void(*g_fcbSetReShapeArrayValue)(wchar_t*, void*, int, int) = NULL;
VariableStruct(*g_fcbGetArrayValue)(wchar_t*) = NULL;
void(*g_fcbPrintMessage)(wchar_t*) = NULL;

CMap<CString, LPCWSTR, VariableStruct*, VariableStruct*> g_DBMapList;

//////////////////////////////////////////////////////////////////////////////////////////////
// enuSpace defualt interface functions.
extern "C" __declspec(dllexport) void SetCallBack_SetValue(void fcbSetValue(wchar_t*, double));
extern "C" __declspec(dllexport) void SetCallBack_GetValue(VariableStruct fcbGetValue(wchar_t*));
extern "C" __declspec(dllexport) void SetCallBack_SetArrayValue(void fcbSetArrayValue(wchar_t*, void*, int, int));
extern "C" __declspec(dllexport) void SetCallBack_GetArrayValue(VariableStruct fcbGetArrayValue(wchar_t*));
extern "C" __declspec(dllexport) void SetCallBack_SetReShapeArrayValue(void fcbSetReShapeArrayValue(wchar_t*, void*, int, int));
extern "C" __declspec(dllexport) void SetCallBack_PrintMessage(void fcbPrintMessage(wchar_t*));

extern "C" __declspec(dllexport) int GetTaskType();
extern "C" __declspec(dllexport) bool IsEnableTransfer(wchar_t* pFromType, wchar_t* pToType);

extern "C" __declspec(dllexport) bool OnInit();
extern "C" __declspec(dllexport) bool OnLoad();
extern "C" __declspec(dllexport) bool OnUnload();
extern "C" __declspec(dllexport) bool OnTask(__int64 time);

extern "C" __declspec(dllexport) void OnEditComponent(wchar_t* pStrSymbolName, wchar_t* pStrID);
extern "C" __declspec(dllexport) void OnShowComponent(wchar_t* pStrSymbolName, wchar_t* pStrID);
//////////////////////////////////////////////////////////////////////////////////////////////
// component call functions.
extern "C" __declspec(dllexport) void SQLite_DB(wchar_t* id, FuncParam* pParam);
extern "C" __declspec(dllexport) void SQLite_CreateTable(wchar_t* id, FuncParam* pParam);
extern "C" __declspec(dllexport) void SQLite_Insert(wchar_t* id, FuncParam* pParam);
extern "C" __declspec(dllexport) void SQLite_Update(wchar_t* id, FuncParam* pParam);
extern "C" __declspec(dllexport) void SQLite_Delete(wchar_t* id, FuncParam* pParam);
extern "C" __declspec(dllexport) void SQLite_Execute(wchar_t* id, FuncParam* pParam);
extern "C" __declspec(dllexport) void SQLite_Select(wchar_t* id, FuncParam* pParam);
extern "C" __declspec(dllexport) void SQLite_Expert(wchar_t* id, FuncParam* pParam);


//////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) void SetCallBack_SetValue(void fcbSetValue(wchar_t*, double))
{
	g_fcbSetValue = fcbSetValue;
}

extern "C" __declspec(dllexport) void SetCallBack_GetValue(VariableStruct fcbGetValue(wchar_t*))
{
	g_fcbGetValue = fcbGetValue;
}

extern "C" __declspec(dllexport) void SetCallBack_SetArrayValue(void fcbSetArrayValue(wchar_t*, void*, int, int))
{
	g_fcbSetArrayValue = fcbSetArrayValue;
}

extern "C" __declspec(dllexport) void SetCallBack_GetArrayValue(VariableStruct fcbGetArrayValue(wchar_t*))
{
	g_fcbGetArrayValue = fcbGetArrayValue;
}

extern "C" __declspec(dllexport) void SetCallBack_SetReShapeArrayValue(void fcbSetReShapeArrayValue(wchar_t*, void*, int, int))
{
	g_fcbSetReShapeArrayValue = fcbSetReShapeArrayValue;
}
extern "C" __declspec(dllexport) void SetCallBack_PrintMessage(void fcbPrintMessage(wchar_t*))
{
	g_fcbPrintMessage = fcbPrintMessage;
}

// 인터페이스 맵 데이터 구조체 클리어 수행.
void InterfaceDataMapClear()
{
	POSITION mappos = g_DBMapList.GetStartPosition();
	while (mappos)
	{
		VariableStruct *pObject = NULL;
		CString srtVar = L"";
		g_DBMapList.GetNextAssoc(mappos, srtVar, pObject);
		if (srtVar != L"")
		{
			if (pObject)
			{
				delete pObject;
			}
		}
	}
	g_DBMapList.RemoveAll();
}

// enuSpace의 메모리 사이즈가 다른경우, enuSpace의 메모리를 재할당후 값 복사하는 함수
// strVariable=ID_OBJECT.input[12][12] (배열의 SHAPE 정보 포함) pSrc=원본 데이터의 포인터, iType=원본 데이터의 타입, iSize=원본 데이터의 사이즈
// 입력된 배열의 정보와 enuSpace의 배열 정보를 비교
void SetReShapeArrayValue(std::string strVariable, void* pSrc, int iType, int iSize)
{
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());

	CString strVal = (wchar_t*)widestr.c_str();

	CString strDim;
	int iPos = strVal.Find(L"[");
	if (iPos > 1)
	{
		strDim = strVal.Right(strVal.GetLength() - iPos);
		strVal = strVal.Left(iPos);
	}

	g_DBMapList.Lookup(strVal, pData);
	if (pData)
	{
		// 요청한 변수의 데이터 타입 및 배열의 길이가 동일하다면, 값을 업데이트 수행.
		if (pData->array.size == iSize && pData->type == iType)
		{
			int itemSize = 0;
			void* pTarget = NULL;
			switch (iType)
			{
			case DEF_BOOL:
			{
				itemSize = iSize * sizeof(bool);
				pTarget = ((bool*)pData->pValue);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_INT:
			{
				itemSize = iSize * sizeof(int);
				pTarget = ((int*)pData->pValue);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_FLOAT:
			{
				itemSize = iSize * sizeof(float);
				pTarget = ((float*)pData->pValue);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_DOUBLE:
			{
				itemSize = iSize * sizeof(double);
				pTarget = ((double*)pData->pValue);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_STRING:
			{
				//*(CString*)pTarget = *((CString*)pData->pValue + iIndex);
				//break;
			}
			}
		}
		// 메모리의 데이터 타입 및 배열의 사이즈가 다르다면, 맵 리스트에서 제거 수행 후 메모리 RESHAPE 후 값 업데이트 함수 호출
		else
		{
			delete pData;
			pData = NULL;
			g_DBMapList.RemoveKey(strVal);

			if (g_fcbSetReShapeArrayValue)
			{
				g_fcbSetReShapeArrayValue((wchar_t*)widestr.c_str(), pSrc, iType, iSize);
			}
			return;
		}
	}

	// 맵 리스트에서 검색을 수행하지 못하였다면, 값을 업데이트 요청함. 값 업데이트 요청함수는 데이터의 타입 및 ARRAY 정보가 다른 경우
	// 메모리의 ReShape 수행후 값을 업데이트 수행
	// 적용된 메모리의 정보를 맵리스트에 추가하여, 다음 로직 수행시 빠른 처리할 수 있도록 추가함.
	if (pData == NULL)
	{
		if (g_fcbGetArrayValue)
		{
			std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());
			VariableStruct Data = g_fcbGetArrayValue(strVal.GetBuffer(0));
			if (Data.pValue)
			{
				// 동일한 데이터의 사이즈 및 타입의 경우 값 업데이트 수행.
				if (Data.type == iType && Data.array.size == iSize)
				{
					int itemSize = 0;
					void* pTarget = NULL;
					switch (iType)
					{
					case DEF_BOOL:
					{
						itemSize = iSize * sizeof(bool);
						pTarget = ((bool*)Data.pValue);
						memcpy(pTarget, pSrc, itemSize);
						break;
					}
					case DEF_INT:
					{
						itemSize = iSize * sizeof(int);
						pTarget = ((int*)Data.pValue);
						memcpy(pTarget, pSrc, itemSize);
						break;
					}
					case DEF_FLOAT:
					{
						itemSize = iSize * sizeof(float);
						pTarget = ((float*)Data.pValue);
						memcpy(pTarget, pSrc, itemSize);
						break;
					}
					case DEF_DOUBLE:
					{
						itemSize = iSize * sizeof(double);
						pTarget = ((double*)Data.pValue);
						memcpy(pTarget, pSrc, itemSize);
						break;
					}
					case DEF_STRING:
					{
						//*(CString*)pTarget = *((CString*)pData->pValue + iIndex);
						//break;
					}
					}
					// 리스트 추가 
					VariableStruct* pNewData = new VariableStruct;
					*pNewData = Data;
					g_DBMapList.SetAt(strVal, pNewData);
				}
				// 입력된 사이즈, 타입이 enuSpace의 사이즈, 타입이 다르다면 RESHAPE 및 값 업데이트 수행
				else
				{
					if (g_fcbSetReShapeArrayValue)
					{
						g_fcbSetReShapeArrayValue((wchar_t*)widestr.c_str(), pSrc, iType, iSize);
					}
				}
			}
			else
			{
				std::string msg = string_format("error : SetArrayValue (Unknown Variable id(%s)", strVariable.c_str());
				PrintMessage(msg);
				return;			// enuSpace에 없는 변수를 요청함.
			}
		}
	}
}

// 배열값 적용 함수 
// strVarialbe=ID_OBJECT.input[0], pSrc=데이터를 적용할 배열의 주소, iType=데이터의 타입, iSize=데이터의 사이즈
// ID_OBJECT.input[10]의 변수가 10개의 Array로 구성시
// ID_OBJECT.input[5], iSize의 값을 3을 입력하였을 경우, 5번째 부터 7번째까지 복사를 수행함.
// 만약 사이즈의 값이 원본 사이즈의 값을 넘어섰을 경우 값을 복사하지 않음.
void SetArrayValue(std::string strVariable, void* pSrc, int iType, int iSize)
{
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());

	CString strVal = (wchar_t*)widestr.c_str();
	CString strDim;

	// 맵의 정보는 배열의 이름은 포함되어 있지 않음. 구조체 정보에 포함됨.
	int iPos = strVal.Find(L"[");
	if (iPos > 1)
	{
		strDim = strVal.Right(strVal.GetLength() - iPos);
		strVal = strVal.Left(iPos);
	}
	g_DBMapList.Lookup(strVal, pData);

	// 맵을 통하여 데이터의 구조체 정보가 없다면, enuSpace에 해당 변수의 포인터 주소를 요청함.
	if (g_fcbSetArrayValue && pData == NULL)
	{
		VariableStruct Data = g_fcbGetArrayValue(strVal.GetBuffer(0));
		if (Data.pValue)
		{
			VariableStruct* pNewData = new VariableStruct;
			*pNewData = Data;
			g_DBMapList.SetAt((wchar_t*)widestr.c_str(), pNewData);

			pData = pNewData;
		}
	}

	// 취득한 메모리의 영역을 검사하고 적절한 범위라면, 복사 수행.
	if (pData && pData->pValue)
	{
		int itemSize = 0;
		void* pTarget = NULL;
		int iIndex = GetArrayIndexFromDimension(pData->array.dimension, strDim);
		if (iIndex + iSize <= pData->array.size)
		{
			switch (iType)
			{
			case DEF_BOOL:
			{
				itemSize = iSize * sizeof(bool);
				pTarget = ((bool*)pData->pValue + iIndex);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_INT:
			{
				itemSize = iSize * sizeof(int);
				pTarget = ((int*)pData->pValue + iIndex);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_FLOAT:
			{
				itemSize = iSize * sizeof(float);
				pTarget = ((float*)pData->pValue + iIndex);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_DOUBLE:
			{
				itemSize = iSize * sizeof(double);
				pTarget = ((double*)pData->pValue + iIndex);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_STRING:
			{
				//*(CString*)pTarget = *((CString*)pData->pValue + iIndex);
				//break;
			}
			}
		}
	}
}

// 배열변수의 값을 요청하는 함수
double GetArrayValue(std::string strVariable)
{
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());

	CString strVal = (wchar_t*)widestr.c_str();
	CString strDim;

	// 맵의 정보는 배열의 이름은 포함되어 있지 않음. 구조체 정보에 포함됨.
	int iPos = strVal.Find(L"[");
	if (iPos > 1)
	{
		strDim = strVal.Right(strVal.GetLength() - iPos);
		strVal = strVal.Left(iPos);
	}
	g_DBMapList.Lookup(strVal, pData);

	if (g_fcbGetArrayValue && pData == NULL)
	{
		std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());
		VariableStruct Data = g_fcbGetArrayValue(strVal.GetBuffer(0));
		if (Data.pValue)
		{
			VariableStruct* pNewData = new VariableStruct;
			*pNewData = Data;
			g_DBMapList.SetAt(strVal, pNewData);

			pData = pNewData;
		}
	}

	double fReturn = 0;
	if (pData && pData->pValue)
	{
		// 원본 배열변수의 배열 정보와 입력된 요청된 배열의 정보를 이용하여 Array의 위치를 획득함.
		int iIndex = GetArrayIndexFromDimension(pData->array.dimension, strDim);
		if (iIndex <= pData->array.size)
		{
			switch (pData->type)
			{
			case DEF_INT:
				fReturn = *(int*)pData->pValue;
				break;
			case DEF_FLOAT:
				fReturn = *(float*)pData->pValue;
				break;
			case DEF_DOUBLE:
				fReturn = *(double*)pData->pValue;
				break;
			case DEF_BOOL:
				if (*(bool*)pData->pValue == TRUE)
					fReturn = 1;
				else
					fReturn = 0;
				break;
			case DEF_STRING:
				fReturn = 0;				// NOT SUPPORT
				break;
			default:
				break;
			}
		}
	}
	return fReturn;
}

void SetValue(std::string strVariable, double fValue)
{
	double fReturn = 0;
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());
	g_DBMapList.Lookup((wchar_t*)widestr.c_str(), pData);

	if (g_fcbSetValue && pData == NULL)
	{
		VariableStruct Data = g_fcbGetValue((wchar_t*)widestr.c_str());
		if (Data.pValue)
		{
			VariableStruct* pNewData = new VariableStruct;
			*pNewData = Data;
			g_DBMapList.SetAt((wchar_t*)widestr.c_str(), pNewData);

			pData = pNewData;
		}
	}

	if (pData && pData->pValue)
	{
		switch (pData->type)
		{
		case DEF_INT:
			*(int*)pData->pValue = (int)fValue;
			break;
		case DEF_FLOAT:
			*(float*)pData->pValue = (float)fValue;
			break;
		case DEF_DOUBLE:
			*(double*)pData->pValue = fValue;
			break;
		case DEF_BOOL:
			if (fValue == 1)
				*(bool*)pData->pValue = true;
			else
				*(bool*)pData->pValue = false;
			break;
		case DEF_STRING:

			break;
		default:
			break;
		}
	}
}

double GetValue(std::string strVariable)
{
	double fReturn = 0;
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());
	g_DBMapList.Lookup((wchar_t*)widestr.c_str(), pData);

	if (g_fcbGetValue && pData == NULL)
	{
		VariableStruct Data = g_fcbGetValue((wchar_t*)widestr.c_str());
		if (Data.type)
		{
			VariableStruct* pNewData = new VariableStruct;
			*pNewData = Data;
			g_DBMapList.SetAt((wchar_t*)widestr.c_str(), pNewData);

			pData = pNewData;
		}
	}

	if (pData && pData->pValue)
	{
		switch (pData->type)
		{
		case DEF_INT:
			fReturn = *(int*)pData->pValue;
			break;
		case DEF_FLOAT:
			fReturn = *(float*)pData->pValue;
			break;
		case DEF_DOUBLE:
			fReturn = *(double*)pData->pValue;
			break;
		case DEF_BOOL:
			if (*(bool*)pData->pValue == TRUE)
				fReturn = 1;
			else
				fReturn = 0;
			break;
		case DEF_STRING:
			fReturn = 0;				// NOT SUPPORT
			break;
		default:
			break;
		}
	}
	return fReturn;
}

void PrintMessage(std::string strMessage)
{
	std::string strTenMessage = string_format("SQLite -> %s", strMessage.c_str());
	if (g_fcbPrintMessage)
	{
		g_fcbPrintMessage(StringToCString(strTenMessage).GetBuffer(0));
	}
}

extern "C" __declspec(dllexport) int GetTaskType()
{
	return TASK_TYPE_FUNCTION_COMPONENT;
}

extern "C" __declspec(dllexport) bool IsEnableTransfer(wchar_t* pFromType, wchar_t* pToType)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) bool OnLoad()
{
	return Load_SQLite();
}

extern "C" __declspec(dllexport) bool OnInit()
{
	return Init_SQLite();
}

extern "C" __declspec(dllexport) bool OnTask(__int64 time)
{
	return Task_SQLite();
}

extern "C" __declspec(dllexport) bool OnUnload()
{
	return Unload_SQLite();
}

extern "C" __declspec(dllexport) void OnEditComponent(wchar_t* pStrSymbolName, wchar_t* pStrID)
{

}
extern "C" __declspec(dllexport) void OnShowComponent(wchar_t* pStrSymbolName, wchar_t* pStrID)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////




extern "C" __declspec(dllexport) void SQLite_DB(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		const VariableStruct* pVarStruct = pParam->GetParam("DBNAME");
		if (pVarStruct)
		{
			if (pVarStruct->type == DEF_STRING)
			{
				CString strValue = *(CString*)pVarStruct->pValue;
				Create_SQLite_DB(CStringToString(strValue));
			}
		}
	}
}

extern "C" __declspec(dllexport) void SQLite_IsExistTable(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		CString strDBName;
		CString strTableName;
		bool bExist = false;

		const VariableStruct* pVarDbStruct = pParam->GetParam("DBNAME");
		if (pVarDbStruct)
		{
			if (pVarDbStruct->type == DEF_STRING)
			{
				strDBName = *(CString*)pVarDbStruct->pValue;
			}
		}
		const VariableStruct* pVarQryStruct = pParam->GetParam("TABLENAME");
		if (pVarQryStruct)
		{
			if (pVarQryStruct->type == DEF_STRING)
			{
				strTableName = *(CString*)pVarQryStruct->pValue;
			}
		}
		/////////////////////////////////////////////////////////////////

		bExist = Call_IsExistTable(CStringToString(strDBName), CStringToString(strTableName));

		/////////////////////////////////////////////////////////////////
		const VariableStruct* pVarStatsStruct = pParam->GetOutput("STATUS");
		if (pVarStatsStruct)
		{
			if (pVarStatsStruct->type == DEF_BOOL)
			{
				*(bool*)pVarStatsStruct->pValue = bExist;
			}
			else
			{
				std::string msg;
				msg = string_format("Call_IsExistTable STATUS variable not string.");
				PrintMessage(msg);
			}
		}
		else
		{
			std::string msg;
			msg = string_format("Call_IsExistTable STATUS variable not find.");
			PrintMessage(msg);
		}
	}
}


extern "C" __declspec(dllexport) void SQLite_CreateTable(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		CString strDBName;
		CString strQuery; 
		bool bTrigger = false;
		CString strStatus;

		const VariableStruct* pVarDbStruct = pParam->GetParam("DBNAME");
		if (pVarDbStruct)
		{
			if (pVarDbStruct->type == DEF_STRING)
			{
				strDBName = *(CString*)pVarDbStruct->pValue;
			}
		}
		const VariableStruct* pVarQryStruct = pParam->GetParam("QUERY");
		if (pVarQryStruct)
		{
			if (pVarQryStruct->type == DEF_STRING)
			{
				strQuery = *(CString*)pVarQryStruct->pValue;
			}
		}

		const VariableStruct* pVarTrigerStruct = pParam->GetParam("TRIGER");
		if (pVarTrigerStruct)
		{
			if (pVarTrigerStruct->type == DEF_BOOL)
			{
				bTrigger = *(bool*)pVarTrigerStruct->pValue;
			}
		}
		/////////////////////////////////////////////////////////////////

		if (bTrigger && strQuery.IsEmpty()==false && strDBName.IsEmpty() == false)
		{
			std::string strResult;
			strResult = Call_CreateTable(CStringToString(strDBName), CStringToString(strQuery));

			/////////////////////////////////////////////////////////////////
			const VariableStruct* pVarDbStruct = pParam->GetOutput("STATUS");
			if (pVarDbStruct)
			{
				if (pVarDbStruct->type == DEF_STRING)
				{
					*(CString*)pVarDbStruct->pValue = StringToCString(strResult);

					*(bool*)pVarTrigerStruct->pValue = false;				// Triger 초기화.
				}
			}
		}
	}
}

extern "C" __declspec(dllexport) void SQLite_Insert(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		CString strDBName;
		CString strQuery;
		bool bTriger = false;

		const VariableStruct* pVarDbStruct = pParam->GetParam("DBNAME");
		if (pVarDbStruct)
		{
			if (pVarDbStruct->type == DEF_STRING)
			{
				strDBName = *(CString*)pVarDbStruct->pValue;
			}
		}

		const VariableStruct* pVarQryStruct = pParam->GetParam("QUERY");
		if (pVarQryStruct)
		{
			if (pVarQryStruct->type == DEF_STRING)
			{
				strQuery = *(CString*)pVarQryStruct->pValue;
			}
		}

		const VariableStruct* pVarTrigerStruct = pParam->GetParam("TRIGER");
		if (pVarTrigerStruct)
		{
			if (pVarTrigerStruct->type == DEF_BOOL)
			{
				bTriger = *(bool*)pVarTrigerStruct->pValue;
			}
		}

		if (bTriger && strDBName.IsEmpty()==false && strQuery.IsEmpty() == false)
		{
			std::string strResult;
			strResult = Call_Insert(CStringToString(strDBName), CStringToString(strQuery));

			// output 
			const VariableStruct* pVarOutStruct = pParam->GetOutput("STATUS");
			if (pVarOutStruct)
			{
				if (pVarOutStruct->type == DEF_STRING)
				{
					*(CString*)pVarOutStruct->pValue = StringToCString(strResult);

					*(bool*)pVarTrigerStruct->pValue = false;				// Triger 초기화.
				}
			}
		}
	}
}

extern "C" __declspec(dllexport) void SQLite_Update(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		CString strDBName;
		CString strQuery;
		bool bTriger = false;

		const VariableStruct* pVarDbStruct = pParam->GetParam("DBNAME");
		if (pVarDbStruct)
		{
			if (pVarDbStruct->type == DEF_STRING)
			{
				strDBName = *(CString*)pVarDbStruct->pValue;
			}
		}

		const VariableStruct* pVarQryStruct = pParam->GetParam("QUERY");
		if (pVarQryStruct)
		{
			if (pVarQryStruct->type == DEF_STRING)
			{
				strQuery = *(CString*)pVarQryStruct->pValue;
			}
		}

		const VariableStruct* pVarTrigerStruct = pParam->GetParam("TRIGER");
		if (pVarTrigerStruct)
		{
			if (pVarTrigerStruct->type == DEF_BOOL)
			{
				bTriger = *(bool*)pVarTrigerStruct->pValue;
			}
		}

		if (bTriger && strDBName.IsEmpty() == false && strQuery.IsEmpty() == false)
		{
			std::string strResult;
			strResult = Call_Update(CStringToString(strDBName), CStringToString(strQuery));

			// output 
			const VariableStruct* pVarOutStruct = pParam->GetOutput("STATUS");
			if (pVarOutStruct)
			{
				if (pVarOutStruct->type == DEF_STRING)
				{
					*(CString*)pVarOutStruct->pValue = StringToCString(strResult);
					*(bool*)pVarTrigerStruct->pValue = false;				// Triger 초기화.
				}
			}
		}
	}
}

extern "C" __declspec(dllexport) void SQLite_Delete(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		CString strDBName;
		CString strQuery;
		bool bTriger = false;

		const VariableStruct* pVarDbStruct = pParam->GetParam("DBNAME");
		if (pVarDbStruct)
		{
			if (pVarDbStruct->type == DEF_STRING)
			{
				strDBName = *(CString*)pVarDbStruct->pValue;
			}
		}

		const VariableStruct* pVarQryStruct = pParam->GetParam("QUERY");
		if (pVarQryStruct)
		{
			if (pVarQryStruct->type == DEF_STRING)
			{
				strQuery = *(CString*)pVarQryStruct->pValue;
			}
		}

		const VariableStruct* pVarTrigerStruct = pParam->GetParam("TRIGER");
		if (pVarTrigerStruct)
		{
			if (pVarTrigerStruct->type == DEF_BOOL)
			{
				bTriger = *(bool*)pVarTrigerStruct->pValue;
			}
		}

		if (bTriger && strDBName.IsEmpty() == false && strQuery.IsEmpty() == false)
		{
			std::string strResult;
			strResult = Call_Delete(CStringToString(strDBName), CStringToString(strQuery));

			// output 
			const VariableStruct* pVarOutStruct = pParam->GetOutput("STATUS");
			if (pVarOutStruct)
			{
				if (pVarOutStruct->type == DEF_STRING)
				{
					*(CString*)pVarOutStruct->pValue = StringToCString(strResult);
					*(bool*)pVarTrigerStruct->pValue = false;				// Triger 초기화.
				}
			}
		}
	}
}

extern "C" __declspec(dllexport) void SQLite_Execute(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		CString strDBName;
		CString strQuery;
		bool bTriger = false;

		const VariableStruct* pVarDbStruct = pParam->GetParam("DBNAME");
		if (pVarDbStruct)
		{
			if (pVarDbStruct->type == DEF_STRING)
			{
				strDBName = *(CString*)pVarDbStruct->pValue;
			}
		}

		const VariableStruct* pVarQryStruct = pParam->GetParam("QUERY");
		if (pVarQryStruct)
		{
			if (pVarQryStruct->type == DEF_STRING)
			{
				strQuery = *(CString*)pVarQryStruct->pValue;
			}
		}

		const VariableStruct* pVarTrigerStruct = pParam->GetParam("TRIGER");
		if (pVarTrigerStruct)
		{
			if (pVarTrigerStruct->type == DEF_BOOL)
			{
				bTriger = *(bool*)pVarTrigerStruct->pValue;
			}
		}

		if (bTriger && strDBName.IsEmpty() == false && strQuery.IsEmpty() == false)
		{
			std::string strResult;
			strResult = Call_Execute(CStringToString(strDBName), CStringToString(strQuery));

			// output 
			const VariableStruct* pVarOutStruct = pParam->GetOutput("STATUS");
			if (pVarOutStruct)
			{
				if (pVarOutStruct->type == DEF_STRING)
				{
					*(CString*)pVarOutStruct->pValue = StringToCString(strResult);
					*(bool*)pVarTrigerStruct->pValue = false;				// Triger 초기화.
				}
			}
		}
	}
}

extern "C" __declspec(dllexport) void SQLite_Select(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		CString strDBName;
		CString strQuery;
		bool bTriger = false;

		const VariableStruct* pVarDbStruct = pParam->GetParam("DBNAME");
		if (pVarDbStruct)
		{
			if (pVarDbStruct->type == DEF_STRING)
			{
				strDBName = *(CString*)pVarDbStruct->pValue;
			}
		}

		const VariableStruct* pVarQryStruct = pParam->GetParam("QUERY");
		if (pVarQryStruct)
		{
			if (pVarQryStruct->type == DEF_STRING)
			{
				strQuery = *(CString*)pVarQryStruct->pValue;
			}
		}

		const VariableStruct* pVarTrigerStruct = pParam->GetParam("TRIGER");
		if (pVarTrigerStruct)
		{
			if (pVarTrigerStruct->type == DEF_BOOL)
			{
				bTriger = *(bool*)pVarTrigerStruct->pValue;
			}
		}

		if (bTriger && strDBName.IsEmpty() == false && strQuery.IsEmpty() == false)
		{
			std::string strResult;
			strResult = Call_Select(CStringToString(strDBName), CStringToString(strQuery));

			// output 
			const VariableStruct* pVarResultStruct = pParam->GetOutput("RESULT");
			if (pVarResultStruct)
			{
				if (pVarResultStruct->type == DEF_STRING)
				{
					*(CString*)pVarResultStruct->pValue = StringToCString(strResult);
					*(bool*)pVarTrigerStruct->pValue = false;				// Triger 초기화.
				}
			}

			const VariableStruct* pVarOutStruct = pParam->GetOutput("STATUS");
			if (pVarOutStruct)
			{
				if (pVarOutStruct->type == DEF_STRING)
				{
					if (strResult == "")
						*(CString*)pVarOutStruct->pValue = StringToCString("Select Fail");
					else
						*(CString*)pVarOutStruct->pValue = StringToCString("Select success");
				}
			}
		}
	}
}

extern "C" __declspec(dllexport) void SQLite_Expert(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		//std::string msg = string_format("test : SQLite_Execute Create");
		//PrintMessage(msg);
	}
}