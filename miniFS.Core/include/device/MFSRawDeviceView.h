#pragma once

#include <cstdint>


/*

class MFSRawDeviceView
为 MFSRawDevice 提供部分视图基类。

    成员函数：
    
    bool MFSRawDeviceView::CanWrite() const
        当在子类中被重写时，返回一个 bool 值表示是否可以向当前的部分视图写入数据。

    uint32_t MFSRawDeviceView::GetSize() const
        当在子类中被重写时，返回当前视图的字节大小。

    uint32_t MFSRawDeviceView::Read(void * lpBuffer, uint32_t offset, uint32_t length)
        当在子类中被重写时，从当前视图中读取数据并保存在给定的缓冲区中。
        @param lpBuffer 要存放读取的数据的缓冲区首地址指针。
        @param offset 从当前视图中开始读取的数据的偏移量。
        @param length 要从当前视图中读取的数据的字节数量。
        @returns 从当前视图中成功读取出的字节数量。

    uint32_t MFSDeviceVew::Write(uint32_t offset, uint32_t length, const void * lpBuffer)
        当在子类中被重写时，向当前视图中写入数据。
        @param offset 要写入的数据在当前视图中的偏移量。
        @param length 要写入的数据的字节长度。
        @param lpBuffer 存放要写入的数据的缓冲区首地址指针。

    void MFSRawDeviceView::Flush()
        当在子类中被重写时，将所有更改写入基础设备。

    void MFSRawDeviceView::Close()
        当在子类中被重写时，将所有更改写入基础设备并关闭当前的视图对象。

*/

class MFSRawDeviceView
{
public:
    virtual bool CanWrite() const = 0;
    virtual uint32_t GetSize() const = 0;

    virtual uint32_t Read(void * lpBuffer, uint32_t offset, uint32_t length) = 0;
    virtual uint32_t Write(uint32_t offset, uint32_t length, const void * lpBuffer) = 0;

    virtual void Flush() = 0;
    virtual void Close() = 0;
};

