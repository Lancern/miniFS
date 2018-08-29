#pragma once

#include "MFSString.h"
#include <Windows.h>

/*

class MFSEncoding
�ṩ�ַ�������֧�֡�

    DWORD MFSEncoding::GetBytesCount(const MFSString & string) const
        ������������дʱ����ȡ�������ַ������������ֽڴ�С��

    DWORD MFSEncoding::GetBytes(LPVOID lpBuffer, DWORD dwBufferSize, const MFSString & string) const
        ������������дʱ�����������ַ�������������Ļ������С�
        @param lpBuffer ��ű�����ַ����Ļ������׵�ַ��
        @param dwBufferSize �������ܿ��ô�С��
        @param string Ҫ������ַ�����
        @return �ַ����������д�뵽�������е��ֽڴ�С��

    MFSString MFSEncoding::GetBytes(LPCVOID lpBuffer, DWORD dwBufferSize) const
        ������������дʱ���Ӹ����Ļ������н����ַ���Ϊ MFSString ����
        @param lpBuffer ��ű�����ַ����Ļ������׵�ַ��
        @param dwBufferSize �������д�ű�����ַ������ֽڳ��ȡ�
        @retrun �������ַ�����

*/

class MFSEncoding
{
public:
    virtual ~MFSEncoding();

    virtual DWORD GetBytesCount(const MFSString & string) const = 0;
    virtual DWORD GetBytes(LPVOID lpBuffer, DWORD dwBufferSize, const MFSString & string) const = 0;
    virtual MFSString GetString(LPCVOID lpBuffer, DWORD dwBufferSize) const = 0;

protected:
    MFSEncoding();
};

