#pragma once

#include "MFSBlockDevice.h"
#include <unordered_map>

/*

class MFSBufferedBlockDevice
��ʾһ�����ڲ�д�������Ŀ��豸��

    ��������

    MFSBufferedBlockDevice::MFSBufferedBlockDevice(MFSRawDevice * rawDevice, DWORD bufferedBlockCount = 64)
        ��һ���ֽ����ԭʼ�豸����һ�� MFSBufferedBlockDevice ����
        @param rawDevice �ֽ������ԭʼ�豸��
        @param bufferedBlockCount Ӧ��������ݿ�������

    ��Ա������
    
    bool MFSBufferedBlockDevice::ReadBlock(LPVOID lpBuffer, UINT64 blockId)
        �ӵ�ǰ�Ŀ��豸�ж�ȡһ�����ݿ顣
        @param lpBuffer ���ն�ȡ�������ݵĻ������׵�ַָ�롣
        @param blockId Ҫ��ȡ�����ݿ��š�
        @returns ����һ�� bool ֵָʾ�Ƿ�ɹ�����˶�ȡ������

    bool MFSBufferedBlockDevice::WriteBlock(UINT64 blockId, LPCVOID lpBuffer)
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
    MFSBufferedBlockDevice(MFSRawDevice * rawDevice, DWORD bufferedBlockCount = 64);
    ~MFSBufferedBlockDevice() override;

    bool ReadBlock(LPVOID lpBuffer, UINT64 blockId) override;
    bool WriteBlock(UINT64 blockId, LPCVOID lpBuffer) override;

    void Flush();
    void Close() override;

private:
    std::unordered_map<UINT64, MFSRawDeviceView *> _wreq;
    DWORD _bufferedBlockCount;

    MFSRawDeviceView * LoadBlockIntoBuffer(UINT64 blockId);
};
