#pragma once

#include <cstdint>
#include <Windows.h>


/*

class MFSDateTime
提供精确到秒的时间表示。

    构造器：

    MFSDateTime::MFSDateTime(int year, int month, int day, int hour, int minute, int second)
        从指定的年、月、日、小时、分钟和秒初始化 MFSDateTime 类的新实例。

    MFSDateTime::MFSDateTime(int year, int month, int day)
        从指定的日期初始化 MFSDateTime 类的新实例。小时、分钟和秒被初始化为午夜时间（00:00:00）。

    MFSDateTime::MFSDateTime(uint64_t timestamp)
        从指定的 UTC 时间戳初始化 MFSDateTime 类的新实例。

    MFSDateTime::MFSDateTime()
        从当前系统时间初始化 MFSDateTime 类的新实例。

    成员函数：

    !> 某些意义显然的成员函数没有列出。

    int MFSDateTime::GetDayOfWeek() const
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
    MFSDateTime(int year, int month, int day, int hour, int minute, int second);
    MFSDateTime(int year, int month, int day);
    MFSDateTime(uint64_t timestamp);
    MFSDateTime();

    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
    int GetHour() const;
    int GetMinute() const;
    int GetSecond() const;

    int GetDayOfWeek() const;

    void SetYear(int year);
    void SetMonth(int month);
    void SetDay(int day);
    void SetDate(int year, int month, int day);

    void SetHour(int hour);
    void SetMinute(int minute);
    void SetSecond(int second);
    void SetTime(int hour, int minute, int second);

    void SetDateTime(int year, int month, int day, int hour, int minute, int second);

    uint64_t GetTimestamp() const;
    void SetTimestamp(uint64_t timestamp);

private:
    int _year, _month, _day, _hour, _minute, _second;

    void SetSystemTimeStruct(const SYSTEMTIME * sysTime);
    void GetSystemTimeStruct(SYSTEMTIME * sysTime) const;
};


uint64_t MFSGetCurrentTimestamp();
