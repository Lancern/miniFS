#pragma once

#include "MFSBlockDevice.h"
#include <cstdint>

/*

class MFSSparseDevice
表示支持稀疏数据块操作的设备。

    成员函数：

    virtual bool MFSSparseDevice::ZeroRegion(uint64_t startOffset, uint64_t length) = 0
        当在子类中重写时，将设备中的某个区域置零。
        @param startOffset 要置零的区域的首字节偏移量。
        @param length 要置零的区域长度。
        @return 返回一个 bool 值表示操作是否成功。

    virtual bool MFSSparseDevice::ZeroAll() = 0
        当在子类中重写时，将整个设备置零。
        @return 返回一个 bool 值表示操作是否成功。

    virtual bool MFSSparseDevice::SupportSparse() const = 0
        当在子类中重写时，返回一个 bool 值代表基础设备是否支持稀疏数据操作。

*/


class MFSSparseDevice
    : public virtual MFSRawDevice
{
public:
    virtual bool ZeroRegion(uint64_t startOffset, uint64_t length) = 0;
    virtual bool ZeroAll() = 0;
    virtual bool SupportSparse() const = 0;
};
