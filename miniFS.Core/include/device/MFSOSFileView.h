#pragma once

#include "MFSRawDeviceView.h"
#include <Windows.h>

/*

class MFSOSFileView
表示由底层操作系统提供的磁盘文件抽象的分部视图。

*/

class MFSOSFileView :
    public MFSRawDeviceView
{
public:
    ~MFSOSFileView();

    bool CanWrite() const override;
    uint32_t GetSize() const override;

    uint32_t Read(void * lpBuffer, uint32_t offset, uint32_t length) override;
    uint32_t Write(uint32_t offset, uint32_t length, const void * lpBuffer) override;

    void Flush() override;
    void Close() override;

    friend class MFSOSFileDevice;

private:
    MFSOSFileView(HANDLE hFileMapping, uint64_t offset, uint32_t size, bool readonly);

    void * _lpFileMappingAddress;
    void * _lpFileMappingBaseAddr;
    uint32_t _viewSize;
    bool _canWrite;
};

