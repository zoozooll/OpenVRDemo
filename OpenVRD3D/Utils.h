#pragma once
#include <stdio.h>
#include <debugapi.h>

inline void DebugLog(_In_z_ _Printf_format_string_ char const* const format, ...)
{
	char debugLogString[512];
	va_list argList;
	__crt_va_start(argList, format);
	sprintf(debugLogString, format, argList);
	__crt_va_end(argList);
	OutputDebugStringA(debugLogString);
}
