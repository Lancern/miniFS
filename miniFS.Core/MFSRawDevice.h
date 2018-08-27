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
        ���������б���дʱ���򿪵�ǰ�豸��һ��������ͼ�����ظ���ͼ����

    void MFSRawDevice::Close()
        ���������б���дʱ���ͷŲ��ر��豸��Դ���ú�������֤���й���ĸ��ľ���ȷд������豸��

*/

class MFSRawDevice
{
public:
    virtual ~MFSRawDevice();

    virtual bool CanWrite() const = 0;
    virtual UINT64 GetTotalSize() const = 0;

    virtual MFSRawDeviceView * OpenView(UINT64 offset, DWORD viewSize) = 0;

    virtual void Close() = 0;

protected:
    MFSRawDevice();
};

