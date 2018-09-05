#pragma once

#include <cstdint>
#include <Windows.h>


/*

class MFSDateTime
提供精确到秒的时间表示。

    构造器：

    MFSDateTime::MFSDateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
        从指定的年、月、日、小时、分钟和秒初始化 MFSDateTime 类的新实例。

    MFSDateTime::MFSDateTime(uint32_t year, uint32_t month, uint32_t day)
        从指定的日期初始化 MFSDateTime 类的新实例。小时、分钟和秒被初始化为午夜时间（00:00:00）。

    MFSDateTime::MFSDateTime(uint64_t timestamp)
        从指定的 UTC 时间戳初始化 MFSDateTime 类的新实例。

    MFSDateTime::MFSDateTime()
        从当前系统时间初始化 MFSDateTime 类的新实例。

    成员函数：

    !> 某些意义显然的成员函数没有列出。

    uint32_t MFSDateTime::GetDayOfWeek() const
        获取日期对应的星期数。

    uint64_t GetTimestamp() const
        获取时间对应的 UTC 时间戳。

    void SetTimestamp(uint64_t timestamp)
        将当前实例对象表示的时间替换为给定的 UTC 时间戳表示的时间。


uint64_t GetCurrentTimestamp()
    获取当前时间对应的 UTC 时间戳。

*/

class MFSDateTime
{
public:
    MFSDateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second);
    MFSDateTime(uint32_t year, uint32_t month, uint32_t day);
    MFSDateTime(uint64_t timestamp);
    MFSDateTime();

    uint32_t GetYear() const;
    uint32_t GetMonth() const;
    uint32_t GetDay() const;
    uint32_t GetHour() const;
    uint32_t GetMinute() const;
    uint32_t GetSecond() const;

    uint32_t GetDayOfWeek() const;

    void SetYear(uint32_t year);
    void SetMonth(uint32_t month);
    void SetDay(uint32_t day);
    void SetDate(uint32_t year, uint32_t month, uint32_t day);

    void SetHour(uint32_t hour);
    void SetMinute(uint32_t minute);
    void SetSecond(uint32_t second);
    void SetTime(uint32_t hour, uint32_t minute, uint32_t second);

    void SetDateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second);

    uint64_t GetTimestamp() const;
    void SetTimestamp(uint64_t timestamp);

private:
    uint32_t _year, _month, _day, _hour, _minute, _second;

    void SetSystemTimeStruct(const SYSTEMTIME * sysTime);
    void GetSystemTimeStruct(SYSTEMTIME * sysTime) const;
};


uint64_t MFSGetCurrentTimestamp();
