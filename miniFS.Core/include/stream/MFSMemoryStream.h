#pragma once

#include "MFSStream.h"


/*

class MFSMemoryStream
表示以一个定长字节缓冲区作为后备的流对象。

*/

class MFSMemoryStream
    : public MFSStream
{
public:
    MFSMemoryStream(LPVOID lpBuffer, DWORD bufferSize);

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
    LPVOID _lpBuffer;
    DWORD _size;
    DWORD _offset;
};
