#pragma once

#include "MFSRawDevice.h"
#include <Windows.h>
#include <vector>

/*

class MFSOSFileDevice
表示由底层操作系统提供的磁盘文件设备。

*/

class MFSOSFileDevice :
    public MFSRawDevice
{
public:
    MFSOSFileDevice(HANDLE hFile, bool readonly);
    ~MFSOSFileDevice();

    HANDLE GetFileHandle() const;

    bool CanWrite() const override;
    uint64_t GetTotalSize() const override;

    MFSRawDeviceView * OpenView(uint64_t offset, uint32_t viewSize, bool readonly) override;

    void Close() override;

private:
    HANDLE _hFile;
    HANDLE _hFileMapping;
    uint64_t _fileSize;
    bool _canWrite;
};

