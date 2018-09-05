#pragma once

#include "MFSRawDevice.h"

/*

class MFSBlockDevice
提供从字节随机设备到块随机设备的转换层。

    构造器：

    MFSBlockDevice::MFSBlockDevice(MFSRawDevice * rawDevice)
        从给定的字节随机原始设备初始化 MFSBlockDevice 对象。

    成员函数：
    
    MFSRawDevice * MFSBlockDevice::GetRawDevice() const
        获取原始的字节随机设备对象。

    uint32_t MFSBlockDevice::GetBlockSize() const
        获取块大小。

    uint32_t MFSBlockDevice::GetBlocksCount() const
        获取块的总数量。

    bool MFSBlockDevice::ReadBlock(void * lpBuffer, uint64_t blockId)
        从设备中读取一个数据块到指定的缓冲区中。
        @param lpBuffer 接收读取的数据的缓冲区首地址指针。
        @param blockId 要读取数据的块编号。

    bool MFSBlockDevice::WriteBlock(uint64_t blockId, const void * lpBuffer)
        将给定缓冲区中的一个数据块的数据写入设备中。
        @param blockId 要写入数据的块编号。
        @param lpBuffer 存放要写入的数据的缓冲区首地址指针。

    void MFSBlockDevice::Close()
        将所有的更改写入基础设备并关闭当前块设备。不会释放原始设备。

*/

class MFSBlockDevice
{
public:
    MFSBlockDevice(MFSRawDevice * rawDevice);

    MFSRawDevice * GetRawDevice() const;
    virtual uint32_t GetBlockSize() const;
    virtual uint64_t GetBlocksCount() const;

    virtual bool ReadBlock(void * lpBuffer, uint64_t blockId);
    virtual bool WriteBlock(uint64_t blockId, const void * lpBuffer);

    virtual void Close();

protected:
    virtual MFSRawDeviceView * OpenBlockView(uint64_t blockId, bool readonly);

private:
    MFSRawDevice * _rawDevice;
};

