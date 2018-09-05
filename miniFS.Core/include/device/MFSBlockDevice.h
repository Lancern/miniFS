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

    uint32_t MFSBlockDevice::GetBlockSize() const
        ��ȡ���С��

    uint32_t MFSBlockDevice::GetBlocksCount() const
        ��ȡ�����������

    bool MFSBlockDevice::ReadBlock(void * lpBuffer, uint64_t blockId)
        ���豸�ж�ȡһ�����ݿ鵽ָ���Ļ������С�
        @param lpBuffer ���ն�ȡ�����ݵĻ������׵�ַָ�롣
        @param blockId Ҫ��ȡ���ݵĿ��š�

    bool MFSBlockDevice::WriteBlock(uint64_t blockId, const void * lpBuffer)
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

    MFSRawDevice * GetRawDevice() const;
    virtual uint32_t GetBlockSize() const;
    virtual uint64_t GetBlocksCount() const;

    virtual bool ReadBlock(void * lpBuffer, uint64_t blockId);
    virtual bool WriteBlock(uint64_t blockId, const void * lpBuffer);

    virtual void Close();

protected:
    virtual MFSRawDeviceView * OpenBlockView(uint64_t blockId, bool readonly);

private:
    MFSRawDevice * _rawDevice;
};

