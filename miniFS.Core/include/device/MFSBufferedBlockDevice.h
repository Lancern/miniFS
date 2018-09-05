#pragma once

#include "MFSBlockDevice.h"
#include <unordered_map>

/*

class MFSBufferedBlockDevice
��ʾһ�����ڲ�д�������Ŀ��豸��

    ��������

    MFSBufferedBlockDevice::MFSBufferedBlockDevice(MFSRawDevice * rawDevice, uint32_t bufferedBlockCount = 64)
        ��һ���ֽ����ԭʼ�豸����һ�� MFSBufferedBlockDevice ����
        @param rawDevice �ֽ������ԭʼ�豸��
        @param bufferedBlockCount Ӧ��������ݿ�������

    ��Ա������
    
    bool MFSBufferedBlockDevice::ReadBlock(void * lpBuffer, uint64_t blockId)
        �ӵ�ǰ�Ŀ��豸�ж�ȡһ�����ݿ顣
        @param lpBuffer ���ն�ȡ�������ݵĻ������׵�ַָ�롣
        @param blockId Ҫ��ȡ�����ݿ��š�
        @returns ����һ�� bool ֵָʾ�Ƿ�ɹ�����˶�ȡ������

    bool MFSBufferedBlockDevice::WriteBlock(uint64_t blockId, const void * lpBuffer)
        ��һ�����ݿ�д�뵽��ǰ�Ŀ��豸�С�
        @param blockId Ҫд�����ݵ����ݿ顣
        @param lpBuffer ���Ҫд�����ݵ����ݿ�Ļ������׵�ַָ�롣

    void MFSBufferedBlockDevice::Flush()
        ǿ�ƽ����и���д������豸�С�

    void MFSBufferedBlockDevice::Close()
        �����и���д������豸�У�Ȼ��رյ�ǰ���豸���ײ��豸���ᱻ�رա�

*/

class MFSBufferedBlockDevice
    : public MFSBlockDevice
{
public:
    MFSBufferedBlockDevice(MFSRawDevice * rawDevice, uint32_t bufferedBlockCount = 64);
    ~MFSBufferedBlockDevice() override;

    bool ReadBlock(void * lpBuffer, uint64_t blockId) override;
    bool WriteBlock(uint64_t blockId, const void * lpBuffer) override;

    void Flush();
    void Close() override;

private:
    std::unordered_map<uint64_t, MFSRawDeviceView *> _wreq;
    uint32_t _bufferedBlockCount;

    MFSRawDeviceView * LoadBlockIntoBuffer(uint64_t blockId);
};
