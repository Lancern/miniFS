#pragma once

#include <cstdint>
#include <Windows.h>


/*

class MFSDateTime
�ṩ��ȷ�����ʱ���ʾ��

    ��������

    MFSDateTime::MFSDateTime(int year, int month, int day, int hour, int minute, int second)
        ��ָ�����ꡢ�¡��ա�Сʱ�����Ӻ����ʼ�� MFSDateTime �����ʵ����

    MFSDateTime::MFSDateTime(int year, int month, int day)
        ��ָ�������ڳ�ʼ�� MFSDateTime �����ʵ����Сʱ�����Ӻ��뱻��ʼ��Ϊ��ҹʱ�䣨00:00:00����

    MFSDateTime::MFSDateTime(uint64_t timestamp)
        ��ָ���� UTC ʱ�����ʼ�� MFSDateTime �����ʵ����

    MFSDateTime::MFSDateTime()
        �ӵ�ǰϵͳʱ���ʼ�� MFSDateTime �����ʵ����

    ��Ա������

    !> ĳЩ������Ȼ�ĳ�Ա����û���г���

    int MFSDateTime::GetDayOfWeek() const
        ��ȡ���ڶ�Ӧ����������

    uint64_t GetTimestamp() const
        ��ȡʱ���Ӧ�� UTC ʱ�����

    void SetTimestamp(uint64_t timestamp)
        ����ǰʵ�������ʾ��ʱ���滻Ϊ������ UTC ʱ�����ʾ��ʱ�䡣


uint64_t GetCurrentTimestamp()
    ��ȡ��ǰʱ���Ӧ�� UTC ʱ�����

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
