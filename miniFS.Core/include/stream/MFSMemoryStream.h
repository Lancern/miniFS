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
    MFSMemoryStream(void * lpBuffer, uint32_t bufferSize);

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
    void * _lpBuffer;
    uint32_t _size;
    uint32_t _offset;
};
