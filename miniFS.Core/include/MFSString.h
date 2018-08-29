#pragma once

#include <Windows.h>
#include <memory>
#include <vector>
#include <iterator>

/*

class MFSString
��ʾһ�� Unicode �ַ�����

    ��������

    MFSString::MFSString()
        ��ʼ�� MFSString ����Ϊһ���ַ�����

    MFSString::MFSString(LPCWSTR psRaw)
        �Ӹ����� C ��� Unicode �ַ������� MFSString �����ʵ����

    MFSString::MFSString(LPCWSTR psBuffer, DWORD length)
        �Ӹ������ַ���������ʼ��ַ���ַ������ȴ��� MFSString �����ʵ����

    MFSString::MFSString(const MFSString & another)
        ���ƹ��졣

    MFSString::MFSString(MFSString && another)
        �ƶ����졣

    ��Ա������

    DWORD MFSString::GetLength() const
        ��ȡ��ǰ�ַ����ĳ��ȡ�

    const WCHAR * MFSString::GetRawString() const
        ��ȡ��ǰ�ַ����� C �ַ�����ʾ��

    int MFSString::CompareTo(const MFSString & another) const
        �Ƚϵ�ǰ���ַ�������һ���������ַ������ֵ���
        ����ǰ�����ֵ����� another ֮ǰ������һ������������ǰ���� another ��ͬ������ 0�����򷵻�һ��������

    int MFSString::IndexOf(const MFSString & substring) const
        ��ȡ�����ַ����ڵ�ǰ�ַ����ĵ�һ�γ���λ�á���ָ�����ַ���û���ڵ�ǰ�ַ����г��֣����� -1��

    int MFSString::LastIndexOf(const MFSString & substring) const
        ��ȡ�����ַ����ڵ�ǰ�ַ��������һ�γ���λ�á���ָ�����ַ���û���ڵ�ǰ�ַ����г��֣����� -1��

    bool MFSString::Contains(const MFSString & substring) const
        ���ָ�����ַ����Ƿ��ڵ�ǰ�ַ����г�������һ�Ρ�

    bool MFSString::StartsWith(const MFSString & prefix) const
        ��鵱ǰ�ַ����Ƿ��ɸ������ַ�����ͷ��

    bool MFSString::EndsWith(const MFSString & suffix) const
        ��鵱ǰ�ַ����Ƿ��ɸ������ַ�����β��

    MFSString MFSString::Concat(const MFSString & another) const
        ���������ַ������ڵ�ǰ�ַ�����ĩβ�������µ��ַ�����

    MFSString Substring(DWORD startOffset) const
        ��ȡ��ǰ�ַ����Ĵ�ĳ��λ�ÿ�ʼһֱ����ǰ�ַ�����β���Ӵ���

    MFSString Substring(DWORD startOffset, DWORD length) const
        ��ȡ��ǰ�ַ����Ĵ�ĳ��λ�ÿ�ʼ������Ϊ length ���Ӵ���

    std::vector<MFSString> MFSString::Split(const std::vector<WCHAR> & separators) const
        ����ǰ�ַ�����ָ���ķָ���Ϊ���޲��Ϊ���ɸ��Ӵ���
        @param separators �ָ�����
        @return ��ǰ�ַ����Էָ���Ϊ���ֳ����Ӵ���

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
