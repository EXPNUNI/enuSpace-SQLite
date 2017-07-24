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
	std::string id;					// �׷��� ���� ���� ID
	int     type;					// �׷��� ���� ���� ��ü Ÿ��
	std::string type_name;			// ��ü�� Ÿ���� ���ڿ� �̸�
	void*   pObject;				// �׷��� ���� ���� Ŭ���� ������

	public:ObjectInfo()
	{
		type = SYMBOL_NONE;
		pObject = nullptr;
	}
};


extern std::string g_strDllPath;
extern std::map<std::string, ObjectInfo* > m_ObjectMapList;

#endif