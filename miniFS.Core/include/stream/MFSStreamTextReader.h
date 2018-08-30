#pragma once

#include "MFSStream.h"
#include "../MFSString.h"
#include <vector>


/*

class MFSStreamTextReader
�ṩһ���ʽ����ȡ�ı������ݵİ���������

    ��������

    MFSStreamTextReader::MFSStreamTextReader(MFSStream * stream)
        �Ӹ������������ʼ��һ���µ� MFSStreamTextReader ����

    ��Ա������

    MFSStream * MFSStreamTextReader::GetStream() const
        ��ȡ����������

    WCHAR MFSStreamTextReader::ReadChar()
        �ӻ������ж�ȡһ���ַ���

    MFSString MFSStreamTextReader::ReadString(const std::vector<WCHAR> & terminators, bool includeTerminator)
        �ӻ������ж�ȡ��һ���Ը����Ľ�������β���ַ�����
        @param includeTerminator һ�� bool ֵָʾ�ָ����Ƿ�Ӧ�����ڷ��ص��ַ����С�

    MFSString MFSStreamTextReader::ReadToken()
        �ӻ������ж�ȡ��һ���Կհ��ַ���β���ַ�����

    MFSString MFSStreamTextReader::ReadLine()
        �ӻ������ж�ȡ��һ���ַ�������ĩ���з��ᱻ�����ڷ��ص��ַ����С�

*/

class MFSStreamTextReader
{
public:
    MFSStreamTextReader(MFSStream * stream);

    MFSStream * GetStream() const;

    WCHAR ReadChar();

    MFSString Read(const std::vector<WCHAR> & terminators, bool includeTerminator);

    MFSString ReadToken();
    MFSString ReadLine();

private:
    MFSStream * _stream;
};
