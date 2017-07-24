#pragma once

#ifndef _GLOBAL_HEADER_
#define _GLOBAL_HEADER_

#include <cstdio>
#include <functional>
#include <string>
#include <vector>
#include "../jsoncpp/include/json/json.h"

#define SYMBOL_NONE						-1
#define SYMBOL_SQLITE_DB 				1

#define SYMBOL_SQLITE_DB_STR			"SQLite_DB"


struct ObjectInfo
{
	std::string id;					// 그래픽 로직 블럭의 ID
	int     type;					// 그래픽 로직 블럭의 객체 타입
	std::string type_name;			// 객체의 타입의 문자열 이름
	void*   pObject;				// 그래픽 로직 블럭의 클랙스 포인터

	public:ObjectInfo()
	{
		type = SYMBOL_NONE;
		pObject = nullptr;
	}
};


extern std::string g_strDllPath;
extern std::map<std::string, ObjectInfo* > m_ObjectMapList;

#endif