#pragma once

#include "MFSStream.h"
#include "../device/MFSBlockDevice.h"
#include <memory>


/*

class MFSBlockStream
表示以一个块设备作为基础设备的流对象。

*/

class MFSBlockStream
    : public MFSStream
{
public:
    MFSBlockStream(MFSBlockDevice * device);
    
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    bool HasNext() const override;

    UINT64 GetLength() const override;
    UINT64 GetPosition() const override;

    MFSBlockDevice * GetDevice() const;
    DWORD GetDeviceBlockSize() const;
    UINT64 GetDeviceBlocksCount() const;

    DWORD Read(LPVOID lpBuffer, DWORD dwBufferSize, DWORD dwNumberOfBytesToRead) override;
    DWORD Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite) override;
    bool Seek(MFSStreamSeekOrigin origin, INT64 offset) override;

    void Flush() override;
    void Close() override;

private:
    MFSBlockDevice * _device;
    std::unique_ptr<UCHAR[]> _buffer;
    DWORD _insideOffset;
    UINT64 _blockOffset;
};
