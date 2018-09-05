#include "../include/MFSString.h"
#include <cwctype>
#include <memory>
#include <wchar.h>

MFSString::MFSString()
{
}

MFSString::MFSString(const wchar_t * psRaw)
{
    _len = wcslen(psRaw);
    wchar_t * pBuffer = new wchar_t[_len + 1];
    wcscpy_s(pBuffer, _len + 1, psRaw);

    _data.reset(pBuffer);
}

MFSString::MFSString(const wchar_t * psBuffer, uint32_t length) 
    : _len(length)
{
    wchar_t * pBuffer = new wchar_t[_len + 1];
    memcpy_s(pBuffer, (_len + 1) * sizeof(wchar_t), psBuffer, _len * sizeof(wchar_t));
	pBuffer[_len] = 0;

    _data.reset(pBuffer);
}

MFSString::MFSString(const MFSString & another)
    : _len(another._len)
{
    wchar_t * pBuffer = new wchar_t[_len + 1];
    memcpy_s(pBuffer, (_len + 1) * sizeof(wchar_t), another._data.get(), (_len + 1) * sizeof(wchar_t));

    _data.reset(pBuffer);
}

MFSString::MFSString(MFSString && another)
    : _len(another._len), _data(std::move(another._data))
{
}


MFSString::~MFSString()
{
}


uint32_t MFSString::GetLength() const
{
    return _len;
}

const wchar_t * MFSString::GetRawString() const
{
    return _data.get();
}

int MFSString::CompareTo(const MFSString & another) const
{
    uint32_t p1 = 0;
    uint32_t p2 = 0;
    while (p1 < this->_len && p2 < another._len)
    {
        wchar_t ch1 = this->_data[p1];
        wchar_t ch2 = another._data[p2];
        if (ch1 != ch2)
            return (static_cast<int>(ch1) - static_cast<int>(ch2));
        
        ++p1;
        ++p2;
    }

    return static_cast<int>(this->GetLength()) - static_cast<int>(another.GetLength());
}

int MFSString::IndexOf(const MFSString & substring) const
{
    if (substring._len > this->_len)
        return -1;

    uint32_t bound = this->_len - substring._len;
    for (uint32_t offset = 0; offset <= bound; ++offset)
    {
        if (ContainsAt(substring, offset))
            return offset;
    }

    return -1;
}

int MFSString::LastIndexOf(const MFSString & substring) const
{
    if (substring._len > this->_len)
        return -1;

    uint32_t bound = this->_len - substring._len;
    for (int offset = bound; offset >= 0; --offset)
    {
        if (ContainsAt(substring, offset))
            return offset;
    }

    return -1;
}

bool MFSString::Contains(const MFSString & substring) const
{
    return IndexOf(substring) != -1;
}

bool MFSString::StartsWith(const MFSString & prefix) const
{
    return ContainsAt(prefix, 0);
}

bool MFSString::EndsWith(const MFSString & suffix) const
{
    if (suffix._len > this->_len)
        return false;

    return ContainsAt(suffix, this->_len - suffix._len);
}

MFSString MFSString::Concat(const MFSString & another) const
{
    uint32_t dwBufferLen = this->_len + another._len + 1;
    wchar_t * pBuffer = new wchar_t[dwBufferLen * sizeof(wchar_t)];

    memcpy_s(pBuffer, 
        dwBufferLen * sizeof(wchar_t), 
        this->_data.get(), 
        this->_len * sizeof(wchar_t));
    memcpy_s(pBuffer + this->_len,
        (dwBufferLen - this->_len) * sizeof(wchar_t),
        another._data.get(),
        (another._len + 1) * sizeof(wchar_t));

    return MFSString(pBuffer);
}

MFSString MFSString::Substring(uint32_t startOffset) const
{
    return MFSString(this->_data.get() + startOffset);
}

MFSString MFSString::Substring(uint32_t startOffset, uint32_t length) const
{
    return MFSString(this->_data.get() + startOffset, length);
}

std::vector<MFSString> MFSString::Split(const std::vector<wchar_t>& separators) const
{
    std::vector<MFSString> result;
    
    uint32_t windowLeft = 0;
    while (windowLeft < this->_len)
    {
        uint32_t windowRight = windowLeft;
		while (windowRight < this->_len &&
			std::find(separators.begin(), separators.end(), _data[windowRight]) == separators.end())
			++windowRight;

        result.emplace_back(_data.get() + windowLeft, windowRight - windowLeft);
        windowLeft = windowRight + 1;
    }

    return result;
}

std::vector<MFSString> MFSString::Split(const std::vector<wchar_t>& separators, bool type) const
{
	std::vector<MFSString> result;

	uint32_t windowLeft = 0;
	while (windowLeft < this->_len)
	{
		uint32_t windowRight = windowLeft;
		while (windowRight < this->_len &&
			std::find(separators.begin(), separators.end(), _data[windowRight]) == separators.end())
			++windowRight;
		if (type)
		{
			if (windowRight - windowLeft != 0)
				result.emplace_back(_data.get() + windowLeft, windowRight - windowLeft);
		}
		windowLeft = windowRight + 1;
	}

	return result;
}

std::vector<MFSString> MFSString::SplitName(const std::vector<wchar_t>& separators) const
{
	std::vector<MFSString> result;
	bool flag = 0;

	uint32_t windowLeft = 0;
	while (windowLeft < this->_len)
	{
		uint32_t windowRight = windowLeft;
		while (windowRight < this->_len &&
			(std::find(separators.begin(), separators.end(), _data[windowRight]) == separators.end()) || flag)
		{
			if (_data[windowRight] == L'\"')
				flag = flag ^ 1;
			++windowRight;
		}
		if (windowRight-windowLeft != 0) 
			result.emplace_back(_data.get() + windowLeft, windowRight - windowLeft);
		windowLeft = windowRight + 1;
	}

	return result;
}

uint32_t MFSString::GetHashCode() const
{
    // Use Jenkins hash function.
    // Wikipedia: https://en.wikipedia.org/wiki/Jenkins_hash_function

    uint32_t hash = 0;
    for (uint32_t i = 0; i < _len; ++i)
    {
        hash += static_cast<uint32_t>(_data[i]);
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

bool MFSString::IsInteger() const
{
    if (_len == 0)
        return false;

    uint32_t i = 0;
    if (_data[0] == L'-')
        ++i;

    for (; i < _len; ++i)
    {
        if (!iswdigit(_data[i]))
            return false;
    }

    return true;
}

MFSString::Iterator MFSString::begin() const
{
    return Iterator(*this);
}

MFSString::Iterator MFSString::end() const
{
    return Iterator(_data.get() + _len);
}

MFSString & MFSString::operator=(const MFSString & another)
{
    this->_len = another._len;

    wchar_t * pBuffer = new wchar_t[_len + 1];
    memcpy_s(pBuffer, (_len + 1) * sizeof(wchar_t), another._data.get(), (_len + 1) * sizeof(wchar_t));
    this->_data.reset(pBuffer);

    return *this;
}

MFSString & MFSString::operator=(MFSString && another)
{
    this->_len = another._len;
    this->_data = std::move(another._data);
    return *this;
}

wchar_t MFSString::operator[](uint32_t offset) const
{
    return _data[offset];
}

bool MFSString::ContainsAt(const MFSString & substring, uint32_t offset) const
{
    uint32_t cap = _len - offset;
    if (substring._len > cap)
        return false;

    const wchar_t * p1 = this->_data.get() + offset;
    const wchar_t * p2 = substring._data.get();
    while (*p2)
    {
        if (*p1++ != *p2++)
            return false;
    }

    return true;
}

bool operator==(const MFSString & s1, const MFSString & s2)
{
    return s1.CompareTo(s2) == 0;
}

bool operator!=(const MFSString & s1, const MFSString & s2)
{
    return s1.CompareTo(s2) != 0;
}

bool operator<(const MFSString & s1, const MFSString & s2)
{
    return s1.CompareTo(s2) < 0;
}

bool operator>(const MFSString & s1, const MFSString & s2)
{
    return s1.CompareTo(s2) > 0;
}

bool operator<=(const MFSString & s1, const MFSString & s2)
{
    return s1.CompareTo(s2) <= 0;
}

bool operator>=(const MFSString & s1, const MFSString & s2)
{
    return s1.CompareTo(s2) >= 0;
}

MFSString operator+(const MFSString & s1, const MFSString & s2)
{
    return s1.Concat(s2);
}

MFSString::Iterator::Iterator(const MFSString & string)
    : _iter(string._data.get())
{
}

MFSString::Iterator::Iterator(const wchar_t * ptr)
    : _iter(ptr)
{
}

wchar_t MFSString::Iterator::operator*() const
{
    return *_iter;
}

wchar_t MFSString::Iterator::operator[](int offset) const
{
    return _iter[offset];
}

MFSString::Iterator & MFSString::Iterator::operator++()
{
    ++this->_iter;
    return *this;
}

MFSString::Iterator MFSString::Iterator::operator++(int)
{
    MFSString::Iterator tmp(*this);
    ++_iter;
    return tmp;
}

MFSString::Iterator & MFSString::Iterator::operator--()
{
    --_iter;
	return *this;
}

MFSString::Iterator MFSString::Iterator::operator--(int)
{
	MFSString::Iterator tmp(*this);
    --_iter;
	return tmp;
}

MFSString::Iterator & MFSString::Iterator::operator+=(int offset)
{
    _iter += offset;
    return *this;
}

MFSString::Iterator & MFSString::Iterator::operator-=(int offset)
{
    _iter -= offset;
    return *this;
}

MFSString::Iterator MFSString::Iterator::operator+(int offset)
{
    MFSString::Iterator tmp(*this);
    tmp += offset;
    return tmp;
}

MFSString::Iterator MFSString::Iterator::operator-(int offset)
{
    MFSString::Iterator tmp(*this);
    tmp -= offset;
    return tmp;
}

bool MFSString::Iterator::operator==(const Iterator & another) const
{
    return this->_iter == another._iter;
}

bool MFSString::Iterator::operator!=(const Iterator & another) const
{
    return this->_iter != another._iter;
}
