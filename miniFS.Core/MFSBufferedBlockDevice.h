#pragma once

#include "MFSBlockDevice.h"
#include <unordered_map>


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

