#include "../include/MFSDateTime.h"
#include <Windows.h>


MFSDateTime::MFSDateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
    : _year(year), _month(month), _day(day), _hour(hour), _minute(minute), _second(second)
{
}

MFSDateTime::MFSDateTime(uint32_t year, uint32_t month, uint32_t day)
    : MFSDateTime(year, month, day, 0, 0, 0)
{
}

MFSDateTime::MFSDateTime(uint64_t timestamp)
{
    SetTimestamp(timestamp);
}

MFSDateTime::MFSDateTime()
{
    SYSTEMTIME nowUtc;
    GetSystemTime(&nowUtc);

    SYSTEMTIME now;
    SystemTimeToTzSpecificLocalTime(NULL, &nowUtc, &now);

    SetSystemTimeStruct(&now);
}

uint32_t MFSDateTime::GetYear() const
{
    return _year;
}

uint32_t MFSDateTime::GetMonth() const
{
    return _month;
}

uint32_t MFSDateTime::GetDay() const
{
    return _day;
}

uint32_t MFSDateTime::GetHour() const
{
    return _hour;
}

uint32_t MFSDateTime::GetMinute() const
{
    return _minute;
}

uint32_t MFSDateTime::GetSecond() const
{
    return _second;
}

uint32_t MFSDateTime::GetDayOfWeek() const
{
    SYSTEMTIME sysTime;
    GetSystemTimeStruct(&sysTime);
    
    FILETIME tmp;
    SystemTimeToFileTime(&sysTime, &tmp);
    FileTimeToSystemTime(&tmp, &sysTime);

    return sysTime.wDayOfWeek;
}

void MFSDateTime::SetYear(uint32_t year)
{
    _year = year;
}

void MFSDateTime::SetMonth(uint32_t month)
{
    _month = month;
}

void MFSDateTime::SetDay(uint32_t day)
{
    _day = day;
}

void MFSDateTime::SetDate(uint32_t year, uint32_t month, uint32_t day)
{
    SetYear(year);
    SetMonth(month);
    SetDay(day);
}

void MFSDateTime::SetHour(uint32_t hour)
{
    _hour = hour;
}

void MFSDateTime::SetMinute(uint32_t minute)
{
    _minute = minute;
}

void MFSDateTime::SetSecond(uint32_t second)
{
    _second = second;
}

void MFSDateTime::SetTime(uint32_t hour, uint32_t minute, uint32_t second)
{
    SetHour(hour);
    SetMinute(minute);
    SetSecond(second);
}

void MFSDateTime::SetDateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
{
    SetDate(year, month, day);
    SetTime(hour, minute, second);
}

uint64_t MFSDateTime::GetTimestamp() const
{
    SYSTEMTIME sysTime;
    GetSystemTimeStruct(&sysTime);
    
    SYSTEMTIME sysTimeUtc;
    TzSpecificLocalTimeToSystemTime(NULL, &sysTime, &sysTimeUtc);

    FILETIME fileTime;
    SystemTimeToFileTime(&sysTimeUtc, &fileTime);

    uint64_t result = (static_cast<uint64_t>(fileTime.dwHighDateTime) << 32) | fileTime.dwLowDateTime;
    return result;
}

void MFSDateTime::SetTimestamp(uint64_t timestamp)
{
    FILETIME fileTime;
    fileTime.dwHighDateTime = timestamp >> 32;
    fileTime.dwLowDateTime = static_cast<DWORD>(timestamp & 0xFFFFFFFF);

    SYSTEMTIME sysTimeUtc;
    FileTimeToSystemTime(&fileTime, &sysTimeUtc);

    SYSTEMTIME sysTime;
    SystemTimeToTzSpecificLocalTime(NULL, &sysTimeUtc, &sysTime);

    SetSystemTimeStruct(&sysTime);
}

MFSString MFSDateTime::GetDateString() const
{
    wchar_t buffer[16];
    swprintf_s(buffer, 16, L"%04u-%02u-%02u", _year, _month, _day);

    return MFSString(buffer);
}

MFSString MFSDateTime::GetTimeString() const
{
    wchar_t buffer[16];
    swprintf_s(buffer, 16, L"%02u:%02u:%02u", _hour, _minute, _second);

    return MFSString(buffer);
}

MFSString MFSDateTime::GetDateTimeString() const
{
    wchar_t buffer[32];
    swprintf_s(buffer, 32, L"%04u-%02u-%02u %02u:%02u:%02u", _year, _month, _day, _hour, _minute, _second);

    return MFSString(buffer);
}

void MFSDateTime::SetSystemTimeStruct(const SYSTEMTIME * sysTime)
{
    _year = sysTime->wYear;
    _month = sysTime->wMonth;
    _day = sysTime->wDay;
    _hour = sysTime->wHour;
    _minute = sysTime->wMinute;
    _second = sysTime->wSecond;
}

void MFSDateTime::GetSystemTimeStruct(SYSTEMTIME * sysTime) const
{
    sysTime->wYear = _year;
    sysTime->wMonth = _month;
    sysTime->wDay = _day;
    sysTime->wHour = _hour;
    sysTime->wMinute = _minute;
    sysTime->wSecond = _second;
    sysTime->wMilliseconds = 0;
}

uint64_t MFSGetCurrentTimestamp()
{
    return MFSDateTime().GetTimestamp();
}
