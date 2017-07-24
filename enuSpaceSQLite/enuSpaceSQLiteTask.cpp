#include "stdafx.h"
#include "enuSpaceSQLite.h"
#include "enuSpaceSQLiteTask.h"
#include "utility_functions.h"

#include "GlobalHeader.h"
#include "../SQLite/SQLite/sqlite3.h"
#include "../jsoncpp/include/json/json.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// dll extern execute functions 
int GetSymbolType(std::string strSymbolName)
{
	int iType = SYMBOL_NONE;
	if (strSymbolName == SYMBOL_SQLITE_DB_STR)
		iType = SYMBOL_SQLITE_DB;

	return iType;
}

void* Create_Symbol(int iSymbol, std::string id, Json::Value pInputItem)
{
	void* pCreate = nullptr;
	switch (iSymbol)
	{
		case SYMBOL_SQLITE_DB:	{	pCreate = Create_SQLite_DB(id);	break;	}
	}
	return pCreate;
}

bool Load_SQLite()
{
	return true;
}

bool Init_SQLite()
{
	ObjectMapClear();
	return true;
}

bool Task_SQLite()
{
	return true;
}

bool Unload_SQLite()
{
	ObjectMapClear();
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// SQLITE

const char* SQLite_error_code_string(int nErrCode)
{
	switch (nErrCode)
	{
		case SQLITE_OK: return "SQLITE_OK";
		case SQLITE_ERROR: return "SQLITE_ERROR";
		case SQLITE_INTERNAL: return "SQLITE_INTERNAL";
		case SQLITE_PERM: return "SQLITE_PERM";
		case SQLITE_ABORT: return "SQLITE_ABORT";
		case SQLITE_BUSY: return "SQLITE_BUSY";
		case SQLITE_LOCKED: return "SQLITE_LOCKED";
		case SQLITE_NOMEM: return "SQLITE_NOMEM";
		case SQLITE_READONLY: return "SQLITE_READONLY";
		case SQLITE_INTERRUPT: return "SQLITE_INTERRUPT";
		case SQLITE_IOERR: return "SQLITE_IOERR";
		case SQLITE_CORRUPT: return "SQLITE_CORRUPT";
		case SQLITE_NOTFOUND: return "SQLITE_NOTFOUND";
		case SQLITE_FULL: return "SQLITE_FULL";
		case SQLITE_CANTOPEN: return "SQLITE_CANTOPEN";
		case SQLITE_PROTOCOL: return "SQLITE_PROTOCOL";
		case SQLITE_EMPTY: return "SQLITE_EMPTY";
		case SQLITE_SCHEMA: return "SQLITE_SCHEMA";
		case SQLITE_TOOBIG: return "SQLITE_TOOBIG";
		case SQLITE_CONSTRAINT: return "SQLITE_CONSTRAINT";
		case SQLITE_MISMATCH: return "SQLITE_MISMATCH";
		case SQLITE_MISUSE: return "SQLITE_MISUSE";
		case SQLITE_NOLFS: return "SQLITE_NOLFS";
		case SQLITE_AUTH: return "SQLITE_AUTH";
		case SQLITE_FORMAT: return "SQLITE_FORMAT";
		case SQLITE_RANGE: return "SQLITE_RANGE";
		case SQLITE_ROW: return "SQLITE_ROW";
		case SQLITE_DONE: return "SQLITE_DONE";
		default: return "UNKNOWN_ERROR";
	}
}

std::string SQLite_error_msg(sqlite3* pDB, const int nErrCode)
{
	const char* szError = sqlite3_errmsg(pDB);

	char* mpszErrMess;
	mpszErrMess = sqlite3_mprintf("%s[%d]: %s", SQLite_error_code_string(nErrCode), nErrCode, szError ? szError : "");

	std::string str(mpszErrMess);
	PrintMessage(str);

	sqlite3_free(mpszErrMess);
	
	return str;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
ObjectInfo* LookupFromObjectMap(std::string strid)
{
	ObjectInfo* pObj = nullptr;
	const std::map<std::string, ObjectInfo*>::const_iterator aLookup = m_ObjectMapList.find(strid);
	const bool bExists = aLookup != m_ObjectMapList.end();
	if (bExists)
	{
		pObj = aLookup->second;
		return pObj;
	}
	else
	{
		std::string msg = string_format("warning : Could not find object(%s) in object map.", strid.c_str());
		PrintMessage(msg);
	}
	return pObj;
}

ObjectInfo* AddObjectMap(void* pCreate, std::string id, int iSymbol, std::string type_name)
{
	if (pCreate)
	{
		ObjectInfo* pObj = nullptr;
		const std::map<std::string, ObjectInfo*>::const_iterator aLookup = m_ObjectMapList.find(id);
		const bool bExists = aLookup != m_ObjectMapList.end();
		if (bExists == false)
		{
			ObjectInfo* pCreateObj = new ObjectInfo;
			pCreateObj->id = id;
			pCreateObj->pObject = pCreate;
			pCreateObj->type = iSymbol;
			pCreateObj->type_name = type_name;

			m_ObjectMapList.insert(std::pair<std::string, ObjectInfo*>(id, pCreateObj));
			return pCreateObj;
		}
	}
	std::string msg = string_format("warning : Could not add to list(all object). existed id(%s).", id.c_str());
	PrintMessage(msg);
	return nullptr;
}

void ObjectMapClear()
{
	std::map<std::string, ObjectInfo*>::iterator vit;
	for (vit = m_ObjectMapList.begin(); vit != m_ObjectMapList.end(); ++vit)
	{
		ObjectInfo* pTar = vit->second;
		if (pTar)
		{
			delete pTar;
			pTar = nullptr;
		}
	}
	m_ObjectMapList.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void* Create_SQLite_DB(std::string id)
{
	// 이미 sqlite 객체가 생성되어 있는지 확인후, 만약 생성된것이 있다면 반환을 수행함.
	ObjectInfo* objinfo = LookupFromObjectMap(id);
	if (objinfo)
		return objinfo->pObject;

	sqlite3* pDB = nullptr;

	std::string strfilename = g_strDllPath + id;
	int nRet = sqlite3_open(strfilename.c_str(), &pDB);
	if (nRet != SQLITE_OK)
	{
		SQLite_error_msg(pDB, nRet);
		return pDB;
	}

	sqlite3_busy_timeout(pDB, 30000);			// 30 second

	if (pDB)
		AddObjectMap(pDB, id, SYMBOL_SQLITE_DB, SYMBOL_SQLITE_DB_STR);

	return pDB;
}

// SELECT COUNT(*) from SQLITE_MASTER WHERE TYPE='TABLE' AND NAME='TABLE_NAME';
bool Call_IsExistTable(std::string dbname, std::string table_name)
{
	bool bExist = false;

	ObjectInfo* objinfo = LookupFromObjectMap(dbname);
	if (objinfo)
	{
		if (objinfo->type == SYMBOL_SQLITE_DB)
		{
			sqlite3* pDB = (sqlite3*)objinfo->pObject;

			std::string strQuery;
			strQuery = string_format("SELECT COUNT(*) from SQLITE_MASTER WHERE TYPE='table' AND NAME='%s';", table_name.c_str());

			const char* szTail = 0;
			sqlite3_stmt* pVM;
			int nRet = sqlite3_prepare(pDB, strQuery.c_str(), -1, &pVM, &szTail);
			if (nRet != SQLITE_OK)
			{
				SQLite_error_msg(pDB, nRet);
				return false;
			}

			nRet = sqlite3_step(pVM);

			if (nRet == SQLITE_DONE)
			{
				SQLite_error_msg(pDB, nRet);
				sqlite3_finalize(pVM);
				return false;
			}
			else if (nRet == SQLITE_ROW)
			{
				int mnCols = sqlite3_column_count(pVM);
				if (mnCols > 0)
				{
					const char* pFieldValue = (const char*)sqlite3_column_text(pVM, 0);
					nRet = atoi(pFieldValue);
					sqlite3_finalize(pVM);
					if (nRet > 0)
						return true;
					else
						return false;
				}
				else
					return false;
			}
			else
			{
				SQLite_error_msg(pDB, nRet);
				sqlite3_finalize(pVM);
				return false;
			}
		}
		else
		{
			std::string msg = string_format("warning : Could not find sqlite db handle type error(%s).", dbname.c_str());
			PrintMessage(msg);
			return false;
		}
	}
	else
	{
		std::string msg = string_format("warning : Could not find sqlite db handle(%s).", dbname.c_str());
		PrintMessage(msg);
		return false;
	}
}

// CREATE TABLE TABLE_NAME (COLUMN1 char not null, COLUMN2 char, COLUMN3 char, COLUMN4 char);
std::string Call_CreateTable(std::string dbname, std::string query)
{
	std::string strResult;
	ObjectInfo* objinfo = LookupFromObjectMap(dbname);
	if (objinfo)
	{
		if (objinfo->type == SYMBOL_SQLITE_DB)
		{
			sqlite3* pDB = (sqlite3*)objinfo->pObject;

			std::string strQuery;
			strQuery = string_format("%s", query.c_str());

			const char* szTail = 0;
			sqlite3_stmt* pVM;
			int nRet = sqlite3_prepare(pDB, strQuery.c_str(), -1, &pVM, &szTail);
			if (nRet != SQLITE_OK)
			{
				strResult = SQLite_error_msg(pDB, nRet);
				return strResult;
			}

			nRet = sqlite3_step(pVM);

			if (nRet == SQLITE_DONE)
			{
				nRet = sqlite3_finalize(pVM);
				return "CreateTable Sucess";
			}
			else if (nRet == SQLITE_ROW)
			{
				nRet = sqlite3_finalize(pVM);
				return "CreateTable Sucess";
			}
			else
			{
				SQLite_error_msg(pDB, nRet);
				nRet = sqlite3_finalize(pVM);
				return "CreateTable Fail";
			}
		}
	}
	return "CreateTable Fail";
}

// INSERT INTO TABLE_NAME(COLUMN1, COLUMN2, COLUMN3, COLUMN4) VALUES('VALUE1', 'VALUE2', 'VALUE3', 'VALUE4');
std::string Call_Insert(std::string dbname, std::string query)
{
	std::string strResult;
	ObjectInfo* objinfo = LookupFromObjectMap(dbname);
	if (objinfo)
	{
		if (objinfo->type == SYMBOL_SQLITE_DB)
		{
			sqlite3* pDB = (sqlite3*)objinfo->pObject;

			std::string strQuery;
			strQuery = string_format("%s", query.c_str());

			const char* szTail = 0;
			sqlite3_stmt* pVM;
			int nRet = sqlite3_prepare(pDB, strQuery.c_str(), -1, &pVM, &szTail);
			if (nRet != SQLITE_OK)
			{
				strResult = SQLite_error_msg(pDB, nRet);
				return strResult;
			}

			nRet = sqlite3_step(pVM);

			if (nRet == SQLITE_DONE)
			{
				nRet = sqlite3_finalize(pVM);
				return "Insert Sucess";
			}
			else if (nRet == SQLITE_ROW)
			{
				nRet = sqlite3_finalize(pVM);
				return "Insert Sucess";
			}
			else
			{
				SQLite_error_msg(pDB, nRet);
				nRet = sqlite3_finalize(pVM);
				return "Insert Fail";
			}
		}
	}
	return "Insert Fail";
}

// UPDATE TABLE_NAME SET COLUMN1='VALUE1', COLUMN2='VALUE2', COLUMN3='VALUE3', COLUMN4='VALUE4' WHERE COLUMN1='VALUE1';
std::string Call_Update(std::string dbname, std::string query)
{
	std::string strResult;
	ObjectInfo* objinfo = LookupFromObjectMap(dbname);
	if (objinfo)
	{
		if (objinfo->type == SYMBOL_SQLITE_DB)
		{
			sqlite3* pDB = (sqlite3*)objinfo->pObject;

			std::string strQuery;
			strQuery = string_format("%s", query.c_str());

			const char* szTail = 0;
			sqlite3_stmt* pVM;
			int nRet = sqlite3_prepare(pDB, strQuery.c_str(), -1, &pVM, &szTail);
			if (nRet != SQLITE_OK)
			{
				strResult = SQLite_error_msg(pDB, nRet);
				return strResult;
			}

			nRet = sqlite3_step(pVM);

			if (nRet == SQLITE_DONE)
			{
				nRet = sqlite3_finalize(pVM);
				return "Update Sucess";
			}
			else if (nRet == SQLITE_ROW)
			{
				nRet = sqlite3_finalize(pVM);
				return "Update Sucess";
			}
			else
			{
				SQLite_error_msg(pDB, nRet);
				nRet = sqlite3_finalize(pVM);
				return "Update Fail";
			}
		}
	}
	return "Update Fail";
}

// DELETE FROM TABLE_NAME WHERE COLUMN1='VALUE1';
std::string Call_Delete(std::string dbname, std::string query)
{
	std::string strResult;
	ObjectInfo* objinfo = LookupFromObjectMap(dbname);
	if (objinfo)
	{
		if (objinfo->type == SYMBOL_SQLITE_DB)
		{
			sqlite3* pDB = (sqlite3*)objinfo->pObject;

			std::string strQuery;
			strQuery = string_format("%s", query.c_str());

			const char* szTail = 0;
			sqlite3_stmt* pVM;
			int nRet = sqlite3_prepare(pDB, strQuery.c_str(), -1, &pVM, &szTail);
			if (nRet != SQLITE_OK)
			{
				strResult = SQLite_error_msg(pDB, nRet);
				return strResult;
			}

			nRet = sqlite3_step(pVM);

			if (nRet == SQLITE_DONE)
			{
				nRet = sqlite3_finalize(pVM);
				return "Delete Sucess";
			}
			else if (nRet == SQLITE_ROW)
			{
				nRet = sqlite3_finalize(pVM);
				return "Delete Sucess";
			}
			else
			{
				SQLite_error_msg(pDB, nRet);
				nRet = sqlite3_finalize(pVM);
				return "Delete Fail";
			}
		}
	}
	return "Delete Fail";
}

std::string Call_Execute(std::string dbname, std::string query)
{
	std::string strResult;
	ObjectInfo* objinfo = LookupFromObjectMap(dbname);
	if (objinfo)
	{
		if (objinfo->type == SYMBOL_SQLITE_DB)
		{
			sqlite3* pDB = (sqlite3*)objinfo->pObject;

			std::string strQuery;
			strQuery = string_format("%s", query.c_str());

			const char* szTail = 0;
			sqlite3_stmt* pVM;
			int nRet = sqlite3_prepare(pDB, strQuery.c_str(), -1, &pVM, &szTail);
			if (nRet != SQLITE_OK)
			{
				strResult = SQLite_error_msg(pDB, nRet);
				return strResult;
			}

			nRet = sqlite3_step(pVM);

			if (nRet == SQLITE_DONE)
			{
				nRet = sqlite3_finalize(pVM);
				return "Execute Sucess";
			}
			else if (nRet == SQLITE_ROW)
			{
				nRet = sqlite3_finalize(pVM);
				return "Execute Sucess";
			}
			else
			{
				SQLite_error_msg(pDB, nRet);
				nRet = sqlite3_finalize(pVM);
				return "Execute Fail";
			}
		}
	}
	return "Delete Fail";
}

// "SELECT * FROM TABLE_NAME;";
// "SELECT * FROM TABLE_NAME WHERE COLUMN1='VALUE1';
std::string Call_Select(std::string dbname, std::string query)
{
	std::string strResult;
	ObjectInfo* objinfo = LookupFromObjectMap(dbname);
	if (objinfo)
	{
		if (objinfo->type == SYMBOL_SQLITE_DB)
		{
			sqlite3* pDB = (sqlite3*)objinfo->pObject;

			std::string strQuery;
			strQuery = string_format("%s", query.c_str());

			const char* szTail = 0;
			sqlite3_stmt* pVM;
			int nRet = sqlite3_prepare(pDB, strQuery.c_str(), -1, &pVM, &szTail);
			if (nRet != SQLITE_OK)
			{
				strResult = SQLite_error_msg(pDB, nRet);
				return strResult;
			}

			nRet = sqlite3_step(pVM);

			if (nRet == SQLITE_DONE)
			{
				nRet = sqlite3_finalize(pVM);
				return "";
			}
			else if (nRet == SQLITE_ROW)
			{
				Json::Value root;

				bool bDBCheck = true;
				int iCols = sqlite3_column_count(pVM);
				Json::Value* Record = new Json::Value[iCols];

				while (bDBCheck)
				{
					for (int i = 0; i < iCols; i++)
					{
						int iType = sqlite3_column_type(pVM, i);
						switch (iType)
						{
							case SQLITE_TEXT:
							{
								char* pStrValue = (char*)sqlite3_column_text(pVM, i);
								Record[i].append(reinterpret_cast<char *>(pStrValue));
								break;
							}
							case SQLITE_INTEGER:
							{
								int iValue = sqlite3_column_int(pVM, i);
								Record[i].append(iValue);
								break;
							}
							case SQLITE_FLOAT:
							{
								double fValue = sqlite3_column_double(pVM, i);
								Record[i].append(fValue);
								break;
							}
							case SQLITE_BLOB:
							{
								int nLen = sqlite3_column_bytes(pVM, i);
								const unsigned char* pStrValue = (const unsigned char*)sqlite3_column_blob(pVM, i);
								char* pBlob = (char*)malloc(sizeof(char)*nLen);
								memcpy(pBlob, pStrValue, sizeof(char)*nLen);
								Record[i].append(pBlob);
								break;
							}
							case SQLITE_NULL:
							{
								Record[i].append("");
								break;
							}
							defalut:
							{
								Record[i].append("");
								break;
							}
						}
					}

					int nRet = sqlite3_step(pVM);

					if (nRet == SQLITE_DONE)
					{
						bDBCheck = false;
					}
					else if (nRet == SQLITE_ROW)
					{
						bDBCheck = true;
					}
					else
					{
						bDBCheck = false;
						SQLite_error_msg(pDB, nRet);
						nRet = sqlite3_finalize(pVM);
						return "";
					}
				}

				for (int i = 0; i < iCols; i++)
				{
					const char* szTemp = sqlite3_column_name(pVM, i);
					root[szTemp] = Record[i];
					//bDBCheck = true;
				}

				nRet = sqlite3_finalize(pVM);
				Json::StyledWriter writer;
				std::string outputConfig = writer.write(root);

				return outputConfig;
			}
			else
			{
				SQLite_error_msg(pDB, nRet);
				nRet = sqlite3_finalize(pVM);
				return "";
			}
		}
	}
	return "";
}