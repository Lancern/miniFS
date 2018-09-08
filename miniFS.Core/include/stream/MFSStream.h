#pragma once

#include <cstdint>

/*

class MFSStream
为流对象提供抽象基类。

    成员函数：

    bool MFSStream::CanRead() const
        当在子类中重写时，返回一个 bool 值指示当前的流对象是否支持读操作。

    bool MFSStream::CanWrite() const
        当在子类中重写时，返回一个 bool 指示当前的流对象是否支持写操作。

    bool MFSStream::CanSeek() const
        当在子类中重写时，返回一个 bool 指示当前的流对象是否支持 seek 操作。

    bool MFSStream::HasNext() const
        当在子类中重写时，返回一个 bool 指示当前的流指针之后是否还有数据存在。

    uint64_t MFSStream::GetLength() const
        当在子类中重写时，返回流的长度。

    uint64_t MFSStream::GetPosition() const   
        当在子类中重写时，返回当前的流指针。

    uint32_t MFSStream::Read(void * lpBuffer, uint32_t count)
        当在子类中重写时，从流中读取数据并存放在给定的缓冲区中。
        @param lpBuffer 存放读取出的数据的缓冲区首地址。
        @param dwBufferSize 缓冲区总大小。
        @param dwNumberOfBytesToRead 要从流中读取的最大字节数量。
        @return 实际从流中读取的字节数量。

    uint32_t MFSStream::Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite)
        当在子类中重写时，将数据写入到当前的流对象中。
        @param lpBuffer 存放要写入的数据的缓冲区。
        @param dwNumberOfBytesToWrite 要写入的字节数量。
        @returns 实际写入的字节数量。

    bool MFSStream::Seek(MFSStreamSeekOrigin origin, INT64 offset)
        当在子类中重写时，移动当前流对象的流指针。
        @param origin 移动指针时的相对参考点。
        @param offset 指针相对于参考点的移动距离。
        @return 返回一个 bool 值指示指针移动是否成功。

    void MFSStream::Flush()
        当在子类中重写时，将所有更改写入基础设备。

    void MFSStream::Close()
        当在子类中重写时，关闭当前的流对象并释放持有的所有资源。


enum MFSStreamSeekOrigin
指示移动流指针时的参考点位置。

    枚举成员：

    Begin
        移动流指针时的参考位置位于流对象开头。

    Relative
        移动流指针时的参考位置位于流对象的当前流指针处。

    End
        移动流指针时的参考位置位于流对象结尾。

*/

enum MFSStreamSeekOrigin
{
    Begin,
    Relative,
    End
};

class MFSStream
{
public:
    virtual bool CanRead() const = 0;
    virtual bool CanWrite() const = 0;
    virtual bool CanSeek() const = 0;

    virtual bool HasNext() const = 0;

    virtual uint64_t GetLength() const = 0;
    virtual uint64_t GetPosition() const = 0;

    virtual uint32_t Read(void * lpBuffer, uint32_t dwBufferSize, uint32_t dwNumberOfBytesToRead) = 0;
    virtual uint32_t Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite) = 0;
    virtual bool Seek(MFSStreamSeekOrigin origin, int64_t offset) = 0;

    virtual void Flush() = 0;
    virtual void Close() = 0;
};
