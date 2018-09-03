#include "../include/MFSDateTime.h"
#include <Windows.h>


MFSDateTime::MFSDateTime(int year, int month, int day, int hour, int minute, int second)
    : _year(year), _month(month), _day(day), _hour(hour), _minute(minute), _second(second)
{
}

MFSDateTime::MFSDateTime(int year, int month, int day)
    : MFSDateTime(year, month, day, 0, 0, 0)
{
}

MFSDateTime::MFSDateTime(uint64_t timestamp)
{
    SetTimestamp(timestamp);
}

MFSDateTime::MFSDateTime()
{
    SYSTEMTIME now;
    GetSystemTime(&now);
    SetSystemTimeStruct(&now);
}

int MFSDateTime::GetYear() const
{
    return _year;
}

int MFSDateTime::GetMonth() const
{
    return _month;
}

int MFSDateTime::GetDay() const
{
    return _day;
}

int MFSDateTime::GetHour() const
{
    return _hour;
}

int MFSDateTime::GetMinute() const
{
    return _minute;
}

int MFSDateTime::GetSecond() const
{
    return _second;
}

int MFSDateTime::GetDayOfWeek() const
{
    SYSTEMTIME sysTime;
    GetSystemTimeStruct(&sysTime);
    
    FILETIME tmp;
    SystemTimeToFileTime(&sysTime, &tmp);
    FileTimeToSystemTime(&tmp, &sysTime);

    return sysTime.wDayOfWeek;
}

void MFSDateTime::SetYear(int year)
{
    _year = year;
}

void MFSDateTime::SetMonth(int month)
{
    _month = month;
}

void MFSDateTime::SetDay(int day)
{
    _day = day;
}

void MFSDateTime::SetDate(int year, int month, int day)
{
    SetYear(year);
    SetMonth(month);
    SetDay(day);
}

void MFSDateTime::SetHour(int hour)
{
    _hour = hour;
}

void MFSDateTime::SetMinute(int minute)
{
    _minute = minute;
}

void MFSDateTime::SetSecond(int second)
{
    _second = second;
}

void MFSDateTime::SetTime(int hour, int minute, int second)
{
    SetHour(hour);
    SetMinute(minute);
    SetSecond(second);
}

void MFSDateTime::SetDateTime(int year, int month, int day, int hour, int minute, int second)
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
    if (!FileTimeToSystemTime(&fileTime, &sysTimeUtc))
        return;

    SYSTEMTIME sysTime;
    SystemTimeToTzSpecificLocalTime(NULL, &sysTimeUtc, &sysTime);

    SetSystemTimeStruct(&sysTime);
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
