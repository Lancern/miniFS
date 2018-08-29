#pragma once

#include "MFSString.h"
#include <Windows.h>

/*

class MFSEncoding
提供字符串编码支持。

    DWORD MFSEncoding::GetBytesCount(const MFSString & string) const
        当在子类中重写时，获取给定的字符串经编码后的字节大小。

    DWORD MFSEncoding::GetBytes(LPVOID lpBuffer, DWORD dwBufferSize, const MFSString & string) const
        当在子类中重写时，将给定的字符串编码入给定的缓冲区中。
        @param lpBuffer 存放编码后字符串的缓冲区首地址。
        @param dwBufferSize 缓冲区总可用大小。
        @param string 要编码的字符串。
        @return 字符串经编码后写入到缓冲区中的字节大小。

    MFSString MFSEncoding::GetBytes(LPCVOID lpBuffer, DWORD dwBufferSize) const
        当在子类中重写时，从给定的缓冲区中解码字符串为 MFSString 对象。
        @param lpBuffer 存放编码后字符串的缓冲区首地址。
        @param dwBufferSize 缓冲区中存放编码后字符串的字节长度。
        @retrun 解码后的字符串。

*/

class MFSEncoding
{
public:
    virtual ~MFSEncoding();

    virtual DWORD GetBytesCount(const MFSString & string) const = 0;
    virtual DWORD GetBytes(LPVOID lpBuffer, DWORD dwBufferSize, const MFSString & string) const = 0;
    virtual MFSString GetString(LPCVOID lpBuffer, DWORD dwBufferSize) const = 0;

protected:
    MFSEncoding();
};

