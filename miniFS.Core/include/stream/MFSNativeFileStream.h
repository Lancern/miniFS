#pragma once

#include "MFSStream.h"


class MFSNativeFileStream
    : public MFSStream
{
public:
    MFSNativeFileStream(HANDLE hFile);
    ~MFSNativeFileStream() override;

    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    bool HasNext() const override;

    UINT64 GetLength() const override;
    UINT64 GetPosition() const override;

    DWORD Read(LPVOID lpBuffer, DWORD dwBufferSize, DWORD dwNumberOfBytesToRead) override;
    DWORD Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite) override;
    bool Seek(MFSStreamSeekOrigin origin, INT64 offset) override;

    void Flush() override;
    void Close() override;

private:
    HANDLE _hFile;
};
