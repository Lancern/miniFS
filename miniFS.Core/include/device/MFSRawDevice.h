#pragma once

#include "MFSRawDeviceView.h"
#include <Windows.h>

/*

class MFSRawDevice
Ϊ֧�ֲ�����ͼ��Partial View����ԭʼ�����豸�ṩ������ࡣ

    ��Ա������

    bool MFSRawDevice::CanWrite() const
        ���������б���дʱ������һ�� bool ֵ��ʾ�Ƿ�����ǰ�豸��д�����ݡ�

    UINT64 MFSRawDevice::GetTotalSize() const
        ���������б���дʱ�����ص�ǰ�豸�а������������������ֽ�Ϊ��λ��

    MFSRawDeviceView * MFSRawDevice::OpenView(UINT64 offset, DWORD viewSize)
        �Ե�ǰ�豸�Ķ�д�������Դ��豸��һ��������ͼ��
        @param offset ��ͼ����ʼƫ������
        @param viewSize ��ͼ���������ݵ��ֽڴ�С��

    MFSRawDeviceView * MFSRawDevice::OpenView(UINT64 offset, DWORD viewSize, bool readonly)
        ���������б���дʱ���򿪵�ǰ�豸��һ��������ͼ�����ظ���ͼ����
        @param offset ��ͼ����ʼƫ������
        @param viewSize ��ͼ���������ݵ��ֽڴ�С��
        @param readonly ��ͼ�Ƿ�Ϊֻ����

    void MFSRawDevice::Close()
        ���������б���дʱ���ͷŲ��ر��豸��Դ���ú�������֤���й���ĸ��ľ���ȷд������豸��

*/

class MFSRawDevice
{
public:
    virtual ~MFSRawDevice();

    virtual bool CanWrite() const = 0;
    virtual UINT64 GetTotalSize() const = 0;

    MFSRawDeviceView * OpenView(UINT64 offset, DWORD viewSize);
    virtual MFSRawDeviceView * OpenView(UINT64 offset, DWORD viewSize, bool readonly) = 0;

    virtual void Close() = 0;

protected:
    MFSRawDevice();
};

