#pragma once

#include <Windows.h>
#include <memory>
#include <vector>
#include <iterator>

/*

class MFSString
表示一个 Unicode 字符串。

    构造器：

    MFSString::MFSString()
        初始化 MFSString 对象为一空字符串。

    MFSString::MFSString(LPCWSTR psRaw)
        从给定的 C 风格 Unicode 字符串创建 MFSString 类的新实例。

    MFSString::MFSString(LPCWSTR psBuffer, DWORD length)
        从给定的字符缓冲区起始地址和字符串长度创建 MFSString 类的新实例。

    MFSString::MFSString(const MFSString & another)
        复制构造。

    MFSString::MFSString(MFSString && another)
        移动构造。

    成员函数：

    DWORD MFSString::GetLength() const
        获取当前字符串的长度。

    const WCHAR * MFSString::GetRawString() const
        获取当前字符串的 C 字符串表示。

    int MFSString::CompareTo(const MFSString & another) const
        比较当前的字符串和另一个给定的字符串的字典序。
        若当前串的字典序在 another 之前，返回一个负数；若当前串与 another 相同，返回 0；否则返回一个正数。

    int MFSString::IndexOf(const MFSString & substring) const
        获取给定字符串在当前字符串的第一次出现位置。若指定的字符串没有在当前字符串中出现，返回 -1。

    int MFSString::LastIndexOf(const MFSString & substring) const
        获取给定字符串在当前字符串的最后一次出现位置。若指定的字符串没有在当前字符串中出现，返回 -1。

    bool MFSString::Contains(const MFSString & substring) const
        检查指定的字符串是否在当前字符串中出现至少一次。

    bool MFSString::StartsWith(const MFSString & prefix) const
        检查当前字符串是否由给定的字符串开头。

    bool MFSString::EndsWith(const MFSString & suffix) const
        检查当前字符串是否由给定的字符串结尾。

    MFSString MFSString::Concat(const MFSString & another) const
        将给定的字符串接在当前字符串的末尾并返回新的字符串。

    MFSString Substring(DWORD startOffset) const
        获取当前字符串的从某个位置开始一直到当前字符串结尾的子串。

    MFSString Substring(DWORD startOffset, DWORD length) const
        获取当前字符串的从某个位置开始、长度为 length 的子串。

    std::vector<MFSString> MFSString::Split(const std::vector<WCHAR> & separators) const
        将当前字符串以指定的分隔符为界限拆分为若干个子串。
        @param separators 分隔符。
        @return 当前字符串以分隔符为界拆分出的子串。

*/

class MFSString
{
public:

    class Iterator
    {
    public:
        using difference_type = size_t;
        using value_type = WCHAR;
        using pointer = WCHAR * ;
        using reference = WCHAR & ;
        using iterator_category = std::random_access_iterator_tag;

        explicit Iterator(const MFSString & string);
        explicit Iterator(const WCHAR * ptr);
        
        WCHAR operator * () const;
        WCHAR operator [] (int offset) const;

        Iterator & operator ++ ();
        Iterator operator ++ (int);
        Iterator & operator -- ();
        Iterator operator -- (int);
        Iterator & operator += (int offset);
        Iterator & operator -= (int offset);
        Iterator operator + (int offset);
        Iterator operator - (int offset);

        bool operator == (const Iterator & another) const;
        bool operator != (const Iterator & another) const;
        
    private:
        const WCHAR * _iter;
    };

    MFSString();
    MFSString(LPCWSTR psRaw);
    MFSString(LPCWSTR psBuffer, DWORD length);
    MFSString(const MFSString & another);
    MFSString(MFSString && another);
    ~MFSString();

    DWORD GetLength() const;
    const WCHAR * GetRawString() const;

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

    Iterator begin() const;
    Iterator end() const;

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
