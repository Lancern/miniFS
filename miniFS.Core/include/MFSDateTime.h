#pragma once

#include <cstdint>
#include <Windows.h>


/*

class MFSDateTime
�ṩ��ȷ�����ʱ���ʾ��

    ��������

    MFSDateTime::MFSDateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
        ��ָ�����ꡢ�¡��ա�Сʱ�����Ӻ����ʼ�� MFSDateTime �����ʵ����

    MFSDateTime::MFSDateTime(uint32_t year, uint32_t month, uint32_t day)
        ��ָ�������ڳ�ʼ�� MFSDateTime �����ʵ����Сʱ�����Ӻ��뱻��ʼ��Ϊ��ҹʱ�䣨00:00:00����

    MFSDateTime::MFSDateTime(uint64_t timestamp)
        ��ָ���� UTC ʱ�����ʼ�� MFSDateTime �����ʵ����

    MFSDateTime::MFSDateTime()
        �ӵ�ǰϵͳʱ���ʼ�� MFSDateTime �����ʵ����

    ��Ա������

    !> ĳЩ������Ȼ�ĳ�Ա����û���г���

    uint32_t MFSDateTime::GetDayOfWeek() const
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
