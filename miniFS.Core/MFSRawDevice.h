#pragma once

#include "MFSRawDeviceView.h"
#include <Windows.h>

/*

class MFSRawDevice
为支持部分视图（Partial View）的原始数据设备提供抽象基类。

    成员函数：

    bool MFSRawDevice::CanWrite() const
        当在子类中被重写时，返回一个 bool 值表示是否能向当前设备中写入数据。

    UINT64 MFSRawDevice::GetTotalSize() const
        当在子类中被重写时，返回当前设备中包含的总数据量，以字节为单位。

    MFSRawDeviceView * MFSRawDevice::OpenView(UINT64 offset, DWORD viewSize)
        当在子类中被重写时，打开当前设备的一个部分视图并返回该视图对象。

    void MFSRawDevice::Close()
        当在子类中被重写时，释放并关闭设备资源。该函数不保证所有挂起的更改均正确写入基础设备。

*/

class MFSRawDevice
{
public:
    virtual ~MFSRawDevice();

    virtual bool CanWrite() const = 0;
    virtual UINT64 GetTotalSize() const = 0;

    virtual MFSRawDeviceView * OpenView(UINT64 offset, DWORD viewSize) = 0;

    virtual void Close() = 0;

protected:
    MFSRawDevice();
};

