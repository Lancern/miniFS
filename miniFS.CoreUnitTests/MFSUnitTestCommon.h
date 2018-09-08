#pragma once


#include <Windows.h>


HANDLE MFSUnitTestOpenPartitionFileHandle(bool openExisting); 
void MFSUnitTestLogLastError(const wchar_t * msg);
