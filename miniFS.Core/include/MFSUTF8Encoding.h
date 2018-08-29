#pragma once

#include "MFSEncoding.h"

/*

class MFSUTF8Encoding
¶Ô×Ö·û´®½øÐÐ UTF8 ±àÂë¡£

*/

class MFSUTF8Encoding
    : public MFSEncoding
{
public:
    MFSUTF8Encoding();
    ~MFSUTF8Encoding();

    DWORD GetBytesCount(const MFSString & string) const override;
    DWORD GetBytes(LPVOID lpBuffer, DWORD dwBufferSize, const MFSString & string) const override;
    MFSString GetString(LPCVOID lpBuffer, DWORD dwBufferSize) const override;
};
