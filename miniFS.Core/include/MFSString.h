#pragma once

#include <Windows.h>
#include <iostream>
#include <memory>
#include <vector>
#include <iterator>
#include <type_traits>

/*

class MFSString
��ʾһ�� Unicode �ַ�����

    ��������

    MFSString::MFSString()
        ��ʼ�� MFSString ����Ϊһ���ַ�����
	
	MFSString::MFSString(const wchar_t * psRaw)
        �Ӹ����� C ��� Unicode �ַ������� MFSString �����ʵ����

    MFSString::MFSString(const wchar_t * psBuffer, uint32_t length)
        �Ӹ������ַ���������ʼ��ַ���ַ������ȴ��� MFSString �����ʵ����

    MFSString::MFSString(const MFSString & another)
        ���ƹ��졣

    MFSString::MFSString(MFSString && another)
        �ƶ����졣

    ��Ա������

    uint32_t MFSString::GetLength() const
        ��ȡ��ǰ�ַ����ĳ��ȡ�

    const wchar_t * MFSString::GetRawString() const
        ��ȡ��ǰ�ַ����� C �ַ�����ʾ��

    bool MFSString::IsEmpty() const
        ��鵱ǰ�ַ����Ƿ�Ϊһ�մ���

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

    MFSString Substring(uint32_t startOffset) const
        ��ȡ��ǰ�ַ����Ĵ�ĳ��λ�ÿ�ʼһֱ����ǰ�ַ�����β���Ӵ���

    MFSString Substring(uint32_t startOffset, uint32_t length) const
        ��ȡ��ǰ�ַ����Ĵ�ĳ��λ�ÿ�ʼ������Ϊ length ���Ӵ���

    std::vector<MFSString> MFSString::Split(const std::vector<wchar_t> & separators) const
        ����ǰ�ַ�����ָ���ķָ���Ϊ���޲��Ϊ���ɸ��Ӵ���
        @param separators �ָ�����
        @return ��ǰ�ַ����Էָ���Ϊ���ֳ����Ӵ���

	std::vector<MFSString> MFSString::Split(const std::vector<wchar_t> & separators, bool removeEmpty) const
		����ǰ�ַ�����ָ���ķָ���Ϊ���޲��Ϊ���ɸ��Ӵ���
		removeEmpty Ϊ true ʱ���г����ַ���������
		@param separators �ָ�����
		@return ��ǰ�ַ����Էָ���Ϊ���ֳ����Ӵ���

	std::vector<MFSString> MFSString::SplitName(const std::vector<wchar_t> & separators) const
		����ǰ�ַ�����ָ���ķָ���Ϊ���޲��Ϊ���ɸ��Ӵ�, ���ڴ����������ַ����ļ���, Ĭ��ȥ�մ���
		@param separators �ָ�����
		@return ��ǰ�ַ����Էָ���Ϊ���ֳ����Ӵ���

    template <typename IntegerT> IntegerT MFSString::ParseInteger() const
        ����ǰ�ַ���ת��Ϊ���ʾ��������

    uint32_t MFSString::GetHashCode() const
        ��ȡ��ǰ�ַ����Ĺ�ϣֵ��

    bool MFSString::IsInteger() const
        ȷ����ǰ���ַ����Ƿ��ʾһ��������


template <typename IntegerT> MFSString MFSGetString(IntegerT value)
    ��һ��������ֵת��Ϊ���ַ�����ʾ��

static MFSString MFSString::GetEmpty()
    ��ȡһ�����ַ�����

struct std::hash<MFSString>
    Ϊ MFSString �ṩ STL ��ϣ��֧�֡�

*/

class MFSString
{
public:

    class Iterator
    {
    public:
        using difference_type = size_t;
        using value_type = wchar_t;
        using reference = wchar_t & ;
        using iterator_category = std::random_access_iterator_tag;

        explicit Iterator(const MFSString & string);
        explicit Iterator(const wchar_t * ptr);
        
        wchar_t operator * () const;
        wchar_t operator [] (int offset) const;

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
        const wchar_t * _iter;
    };

    MFSString();
    MFSString(wchar_t ch);
    MFSString(const wchar_t * psRaw);
    MFSString(const wchar_t * psBuffer, uint32_t length);
    MFSString(const MFSString & another);
    MFSString(MFSString && another);
    ~MFSString();

    uint32_t GetLength() const;
    const wchar_t * GetRawString() const;
    bool IsEmpty() const;

    int CompareTo(const MFSString & another) const;

    int IndexOf(const MFSString & substring) const;
    int LastIndexOf(const MFSString & substring) const;
    bool Contains(const MFSString & substring) const;
    bool StartsWith(const MFSString & prefix) const;
    bool EndsWith(const MFSString & suffix) const;

    MFSString Concat(const MFSString & another) const;

    MFSString Substring(uint32_t startOffset) const;
    MFSString Substring(uint32_t startOffset, uint32_t length) const;

    std::vector<MFSString> Split(const std::vector<wchar_t> & separators) const;
	std::vector<MFSString> Split(const std::vector<wchar_t> & separators, bool removeEmpty) const;
	std::vector<MFSString> SplitName(const std::vector<wchar_t> & separators) const;

    uint32_t GetHashCode() const;

    bool IsInteger() const;

    template <typename IntegerT>
    IntegerT ParseInteger() const;

    Iterator begin() const;
    Iterator end() const;

    MFSString & operator = (const MFSString & another);
    MFSString & operator = (MFSString && another);
    wchar_t operator [] (uint32_t offset) const;

    template <typename ...T>
    static MFSString Format(const wchar_t * format, T...args);

    static MFSString GetEmpty();

private:
    uint32_t _len;
    std::unique_ptr<wchar_t[]> _data;

    bool ContainsAt(const MFSString & substring, uint32_t offset) const;
};

bool operator == (const MFSString & s1, const MFSString & s2);
bool operator != (const MFSString & s1, const MFSString & s2);
bool operator < (const MFSString & s1, const MFSString & s2);
bool operator > (const MFSString & s1, const MFSString & s2);
bool operator <= (const MFSString & s1, const MFSString & s2);
bool operator >= (const MFSString & s1, const MFSString & s2);
MFSString operator + (const MFSString & s1, const MFSString & s2);

std::wostream & operator << (std::wostream & stream, MFSString & string);
std::wistream & operator >> (std::wistream & stream, MFSString & string);

template <typename IntegerT>
MFSString MFSGetString(IntegerT value)
{
    static_assert(std::is_integral_v<IntegerT>, "The type given should be a integer type.");

    uint32_t valueLen = 0;
    if constexpr (std::is_signed_v<IntegerT>)
    {
        if (value < 0)
            valueLen = 1;
    }

    if (value == 0)
        ++valueLen;
    for (IntegerT tmp = value; tmp; tmp /= 10)
        ++valueLen;

    wchar_t * lpBuffer = new wchar_t[valueLen + 1];
    wchar_t * lpBufferBase = lpBuffer;
    if constexpr (std::is_signed_v<IntegerT>)
    {
        if (value < 0)
        {
            *lpBuffer++ = L'-';
            value = -value;
        }
    }

    if (value == 0)
        *lpBuffer++ = L'0';

    IntegerT level = 1;
    while (value / 10 >= level)
        level *= 10;

    while (value)
    {
        IntegerT d = value / level;
        *lpBuffer++ = L'0' + d;
        value /= 10;
        level /= 10;
    }

    *lpBuffer = 0;
    MFSString result(lpBuffer);

    delete[] lpBufferBase;
    return result;
}

template<typename IntegerT>
inline IntegerT MFSString::ParseInteger() const
{
    static_assert(std::is_integral_v<IntegerT>, "The type given should be a integer type.");

    IntegerT result = 0;

    if (_len == 0)
        return 0;

    IntegerT sign = 1;
    uint32_t i = 0;
    if (_data[i] == '-')
    {
        sign = static_cast<IntegerT>(-1);
        ++i;
    }

    for (; i < _len; ++i)
    {
        IntegerT d = _data[i] - L'0';
        result = result * 10 + d;
    }

    return result * sign;
}

template <typename ...T>
MFSString MFSString::Format(const wchar_t * format, T... arg)
{
    uint32_t bufferSize = 64;
    while (true)
    {
        std::unique_ptr<wchar_t[]> buffer(new wchar_t[bufferSize]);
        if (swprintf_s(buffer.get(), bufferSize, format, arg...) != -1)
            return MFSString(buffer.get());
    }
    return MFSString();
}

namespace std
{
    template <>
    struct hash<MFSString>
    {
        size_t operator () (const MFSString & string) const;
    };

    inline size_t hash<MFSString>::operator()(const MFSString & string) const
    {
        return static_cast<size_t>(string.GetHashCode());
    }
}
