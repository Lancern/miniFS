#pragma once

#include "MFSStream.h"
#include <Windows.h>


class MFSNativeFileStream
    : public MFSStream
{
public:
    MFSNativeFileStream(HANDLE hFile);
    ~MFSNativeFileStream();

    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    bool HasNext() const override;

    uint64_t GetLength() const override;
    uint64_t GetPosition() const override;

    uint32_t Read(void * lpBuffer, uint32_t dwBufferSize, uint32_t dwNumberOfBytesToRead) override;
    uint32_t Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite) override;
    bool Seek(MFSStreamSeekOrigin origin, int64_t offset) override;

    void Flush() override;
    void Close() override;

private:
    HANDLE _hFile;
};
