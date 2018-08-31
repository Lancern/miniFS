#include "../include/MFSString.h"
#include <cwctype>

MFSString::MFSString()
{
}

MFSString::MFSString(LPCWSTR psRaw)
{
    _len = wcslen(psRaw);
    WCHAR * pBuffer = new WCHAR[_len + 1];
    wcscpy_s(pBuffer, _len + 1, psRaw);

    _data.reset(pBuffer);
}

MFSString::MFSString(LPCWSTR psBuffer, DWORD length) 
    : _len(length)
{
    WCHAR * pBuffer = new WCHAR[_len + 1];
    memcpy_s(pBuffer, (_len + 1) * sizeof(WCHAR), psBuffer, _len * sizeof(WCHAR));
	pBuffer[_len] = 0;

    _data.reset(pBuffer);
}

MFSString::MFSString(const MFSString & another)
    : _len(another._len)
{
    WCHAR * pBuffer = new WCHAR[_len + 1];
    memcpy_s(pBuffer, (_len + 1) * sizeof(WCHAR), another._data.get(), (_len + 1) * sizeof(WCHAR));

    _data.reset(pBuffer);
}

MFSString::MFSString(MFSString && another)
    : _len(another._len), _data(std::move(another._data))
{
}


MFSString::~MFSString()
{
}


DWORD MFSString::GetLength() const
{
    return _len;
}

const WCHAR * MFSString::GetRawString() const
{
    return _data.get();
}

int MFSString::CompareTo(const MFSString & another) const
{
    DWORD p1 = 0;
    DWORD p2 = 0;
    while (p1 < this->_len && p2 < another._len)
    {
        WCHAR ch1 = this->_data[p1];
        WCHAR ch2 = another._data[p2];
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

    DWORD bound = this->_len - substring._len;
    for (DWORD offset = 0; offset <= bound; ++offset)
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

    DWORD bound = this->_len - substring._len;
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
    DWORD dwBufferLen = this->_len + another._len + 1;
    WCHAR * pBuffer = new WCHAR[dwBufferLen * sizeof(WCHAR)];

    memcpy_s(pBuffer, 
        dwBufferLen * sizeof(WCHAR), 
        this->_data.get(), 
        this->_len * sizeof(WCHAR));
    memcpy_s(pBuffer + this->_len,
        (dwBufferLen - this->_len) * sizeof(WCHAR),
        another._data.get(),
        (another._len + 1) * sizeof(WCHAR));

    return MFSString(pBuffer);
}

MFSString MFSString::Substring(DWORD startOffset) const
{
    return MFSString(this->_data.get() + startOffset);
}

MFSString MFSString::Substring(DWORD startOffset, DWORD length) const
{
    return MFSString(this->_data.get() + startOffset, length);
}

std::vector<MFSString> MFSString::Split(const std::vector<WCHAR>& separators) const
{
    std::vector<MFSString> result;
    
    DWORD windowLeft = 0;
    while (windowLeft < this->_len)
    {
        DWORD windowRight = windowLeft;
		while (windowRight < this->_len &&
			std::find(separators.begin(), separators.end(), _data[windowRight]) == separators.end())
			++windowRight;

        result.emplace_back(_data.get() + windowLeft, windowRight - windowLeft);
        windowLeft = windowRight + 1;
    }

    return result;
}

std::vector<MFSString> MFSString::Split(const std::vector<WCHAR>& separators, bool type) const
{
	std::vector<MFSString> result;

	DWORD windowLeft = 0;
	while (windowLeft < this->_len)
	{
		DWORD windowRight = windowLeft;
		while (windowRight < this->_len &&
			std::find(separators.begin(), separators.end(), _data[windowRight]) == separators.end())
			++windowRight;
		if (type == TRUE)
		{
			if (windowRight - windowLeft != 0)
				result.emplace_back(_data.get() + windowLeft, windowRight - windowLeft);
		}
		windowLeft = windowRight + 1;
	}

	return result;
}

std::vector<MFSString> MFSString::SplitName(const std::vector<WCHAR>& separators) const
{
	std::vector<MFSString> result;
	bool flag = 0;

	DWORD windowLeft = 0;
	while (windowLeft < this->_len)
	{
		DWORD windowRight = windowLeft;
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

UINT32 MFSString::GetHashCode() const
{
    // Use Jenkins hash function.
    // Wikipedia: https://en.wikipedia.org/wiki/Jenkins_hash_function

    UINT32 hash = 0;
    for (DWORD i = 0; i < _len; ++i)
    {
        hash += static_cast<UINT32>(_data[i]);
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

    DWORD i = 0;
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

    WCHAR * pBuffer = new WCHAR[_len + 1];
    memcpy_s(pBuffer, (_len + 1) * sizeof(WCHAR), another._data.get(), (_len + 1) * sizeof(WCHAR));
    this->_data.reset(pBuffer);

    return *this;
}

MFSString & MFSString::operator=(MFSString && another)
{
    this->_len = another._len;
    this->_data = std::move(another._data);
    return *this;
}

WCHAR MFSString::operator[](DWORD offset) const
{
    return _data[offset];
}

bool MFSString::ContainsAt(const MFSString & substring, DWORD offset) const
{
    DWORD cap = _len - offset;
    if (substring._len > cap)
        return false;

    const WCHAR * p1 = this->_data.get() + offset;
    const WCHAR * p2 = substring._data.get();
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

MFSString::Iterator::Iterator(const WCHAR * ptr)
    : _iter(ptr)
{
}

WCHAR MFSString::Iterator::operator*() const
{
    return *_iter;
}

WCHAR MFSString::Iterator::operator[](int offset) const
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
