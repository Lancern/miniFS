#include "../include/MFSUTF8Encoding.h"


MFSUTF8Encoding::MFSUTF8Encoding()
{
}

MFSUTF8Encoding::~MFSUTF8Encoding()
{
}


DWORD MFSUTF8Encoding::GetBytesCount(const MFSString & string) const
{
    return WideCharToMultiByte(CP_UTF8, 0, string.GetRawString(), string.GetLength(), NULL, 0, NULL, NULL);
}

DWORD MFSUTF8Encoding::GetBytes(LPVOID lpBuffer, DWORD dwBufferSize, const MFSString & string) const
{
    return WideCharToMultiByte(CP_UTF8, 0, string.GetRawString(), string.GetLength(),
        static_cast<LPSTR>(lpBuffer), dwBufferSize, NULL, NULL);
}

MFSString MFSUTF8Encoding::GetString(LPCVOID lpBuffer, DWORD dwBufferSize) const
{
    DWORD length = MultiByteToWideChar(CP_UTF8, 0,
        static_cast<LPCCH>(lpBuffer), dwBufferSize, NULL, 0);
    WCHAR * lpResultBuffer = new WCHAR[length];

    MultiByteToWideChar(CP_UTF8, 0,
        static_cast<LPCCH>(lpBuffer), dwBufferSize, lpResultBuffer, length);
    return MFSString(lpResultBuffer);
}

