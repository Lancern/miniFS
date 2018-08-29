#pragma once

#include "MFSString.h"
#include <Windows.h>

/*

class MFSEncoding
�ṩ�ַ������롣

*/

class MFSEncoding
{
public:
    virtual ~MFSEncoding();

    virtual DWORD GetBytesCount(const MFSString & string) const = 0;
    virtual void GetBytes(const MFSString & string) const = 0;
    virtual MFSString GetString(LPVOID lpBuffer, DWORD dwBufferSize) const = 0;

protected:
    MFSEncoding();
};

