#pragma once

#include "MFSRawDeviceView.h"
#include <cstdint>

/*

class MFSRawDevice
为支持部分视图（Partial View）的原始数据设备提供抽象基类。

    成员函数：

    bool MFSRawDevice::CanWrite() const
        当在子类中被重写时，返回一个 bool 值表示是否能向当前设备中写入数据。

    uint64_t MFSRawDevice::GetTotalSize() const
        当在子类中被重写时，返回当前设备中包含的总数据量，以字节为单位。

    MFSRawDeviceView * MFSRawDevice::OpenView(uint64_t offset, uint32_t viewSize)
        以当前设备的读写控制属性打开设备的一个部分视图。
        @param offset 视图的起始偏移量。
        @param viewSize 视图包含的数据的字节大小。

    MFSRawDeviceView * MFSRawDevice::OpenView(uint64_t offset, uint32_t viewSize, bool readonly)
        当在子类中被重写时，打开当前设备的一个部分视图并返回该视图对象。
        @param offset 视图的起始偏移量。
        @param viewSize 视图包含的数据的字节大小。
        @param readonly 视图是否为只读。

    void MFSRawDevice::Close()
        当在子类中被重写时，释放并关闭设备资源。该函数不保证所有挂起的更改均正确写入基础设备。

*/

class MFSRawDevice
{
public:
    virtual ~MFSRawDevice();

    virtual bool CanWrite() const = 0;
    virtual uint64_t GetTotalSize() const = 0;

    MFSRawDeviceView * OpenView(uint64_t offset, uint32_t viewSize);
    virtual MFSRawDeviceView * OpenView(uint64_t offset, uint32_t viewSize, bool readonly) = 0;

    virtual void Close() = 0;

protected:
    MFSRawDevice();
};

