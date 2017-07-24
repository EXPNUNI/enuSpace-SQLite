#pragma once

#include "GlobalHeader.h"
#include "../SQLite/SQLite/sqlite3.h"
#include "../jsoncpp/include/json/json.h"

///////////////////////////////////////////////////////////////////////
// util functions
ObjectInfo* LookupFromObjectMap(std::string strid);
ObjectInfo* AddObjectMap(void* pCreate, std::string id, int iSymbol, std::string type_name);
void ObjectMapClear();

int GetSymbolType(std::string strSymbolName);
void* Create_Symbol(int iSymbol, std::string id, Json::Value pInputItem);

const char* SQLite_error_code_string(int nErrCode);
std::string SQLite_error_msg(sqlite3* pDB, const int nErrCode);

///////////////////////////////////////////////////////////////////////
// default interface functions.
bool Load_SQLite();
bool Init_SQLite();
bool Task_SQLite();
bool Unload_SQLite();

// interface functions
void* Create_SQLite_DB(std::string id);
bool Call_IsExistTable(std::string dbname, std::string table_name);
std::string Call_CreateTable(std::string dbname, std::string query);
std::string Call_Insert(std::string dbname, std::string query);
std::string Call_Update(std::string dbname, std::string query);
std::string Call_Delete(std::string dbname, std::string query);
std::string Call_Execute(std::string dbname, std::string query);
std::string Call_Select(std::string dbname, std::string query);