#pragma once

#include "MFSRawDevice.h"

/*

class MFSBlockDevice
�ṩ���ֽ�����豸��������豸��ת���㡣

    ��������

    MFSBlockDevice::MFSBlockDevice(MFSRawDevice * rawDevice)
        �Ӹ������ֽ����ԭʼ�豸��ʼ�� MFSBlockDevice ����

    ��Ա������
    
    MFSRawDevice * MFSBlockDevice::GetRawDevice() const
        ��ȡԭʼ���ֽ�����豸����

    DWORD MFSBlockDevice::GetBlockSize() const
        ��ȡ���С��

    DWORD MFSBlockDevice::GetBlocksCount() const
        ��ȡ�����������

    bool MFSBlockDevice::ReadBlock(LPVOID lpBuffer, UINT64 blockId)
        ���豸�ж�ȡһ�����ݿ鵽ָ���Ļ������С�
        @param lpBuffer ���ն�ȡ�����ݵĻ������׵�ַָ�롣
        @param blockId Ҫ��ȡ���ݵĿ��š�

    bool MFSBlockDevice::WriteBlock(UINT64 blockId, LPCVOID lpBuffer)
        �������������е�һ�����ݿ������д���豸�С�
        @param blockId Ҫд�����ݵĿ��š�
        @param lpBuffer ���Ҫд������ݵĻ������׵�ַָ�롣

    void MFSBlockDevice::Close()
        �����еĸ���д������豸���رյ�ǰ���豸�������ͷ�ԭʼ�豸��

*/

class MFSBlockDevice
{
public:
    MFSBlockDevice(MFSRawDevice * rawDevice);
    virtual ~MFSBlockDevice();

    MFSRawDevice * GetRawDevice() const;
    virtual DWORD GetBlockSize() const;
    virtual UINT64 GetBlocksCount() const;

    virtual bool ReadBlock(LPVOID lpBuffer, UINT64 blockId);
    virtual bool WriteBlock(UINT64 blockId, LPCVOID lpBuMFSffer);

    virtual void Close();

protected:
    virtual MFSRawDeviceView * OpenBlockView(UINT64 blockId, bool readonly);

private:
    MFSRawDevice * _rawDevice;
};

