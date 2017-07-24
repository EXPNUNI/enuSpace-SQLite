#pragma once


#include <string>
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <vector>

std::string string_format(const std::string fmt_str, ...);
CString StringToCString(std::string str);
std::string CStringToString(CString reqStr);
int GetArrayIndexFromDimension(CString strOrgDim, CString strDimension);