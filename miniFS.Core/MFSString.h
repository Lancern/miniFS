#pragma once

#include <Windows.h>
#include <memory>
#include <vector>
#include <array>

/*

class MFSString
表示一个 Unicode 字符串。

*/

class MFSString
{
public:
    MFSString();
    MFSString(LPCWSTR psRaw);
    MFSString(LPCWSTR psBuffer, DWORD length);
    MFSString(const MFSString & another);
    MFSString(MFSString && another);
    ~MFSString();

    DWORD GetLength() const;

    int CompareTo(const MFSString & another) const;

    int IndexOf(const MFSString & substring) const;
    int LastIndexOf(const MFSString & substring) const;
    bool Contains(const MFSString & substring) const;
    bool StartsWith(const MFSString & prefix) const;
    bool EndsWith(const MFSString & suffix) const;

    MFSString Concat(const MFSString & another) const;

    MFSString Substring(DWORD startOffset) const;
    MFSString Substring(DWORD startOffset, DWORD length) const;

    std::vector<MFSString> Split(const std::vector<WCHAR> & separators) const;

    MFSString & operator = (const MFSString & another);
    MFSString & operator = (MFSString && another);
    WCHAR operator [] (DWORD offset) const;

private:
    DWORD _len;
    std::unique_ptr<WCHAR[]> _data;

    bool ContainsAt(const MFSString & substring, DWORD offset) const;
};

bool operator == (const MFSString & s1, const MFSString & s2);
bool operator != (const MFSString & s1, const MFSString & s2);
bool operator < (const MFSString & s1, const MFSString & s2);
bool operator > (const MFSString & s1, const MFSString & s2);
bool operator <= (const MFSString & s1, const MFSString & s2);
bool operator >= (const MFSString & s1, const MFSString & s2);
MFSString operator + (const MFSString & s1, const MFSString & s2);
