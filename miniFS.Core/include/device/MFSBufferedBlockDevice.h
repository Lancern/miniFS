#pragma once

#include "MFSBlockDevice.h"
#include <unordered_map>

/*

class MFSBufferedBlockDevice
表示一个带内部写缓冲区的块设备。

    构造器：

    MFSBufferedBlockDevice::MFSBufferedBlockDevice(MFSRawDevice * rawDevice, uint32_t bufferedBlockCount = 64)
        从一个字节随机原始设备创建一个 MFSBufferedBlockDevice 对象。
        @param rawDevice 字节随机的原始设备。
        @param bufferedBlockCount 应缓冲的数据块数量。

    成员函数：
    
    bool MFSBufferedBlockDevice::ReadBlock(void * lpBuffer, uint64_t blockId)
        从当前的块设备中读取一个数据块。
        @param lpBuffer 接收读取出的数据的缓冲区首地址指针。
        @param blockId 要读取的数据块编号。
        @returns 返回一个 bool 值指示是否成功完成了读取操作。

    bool MFSBufferedBlockDevice::WriteBlock(uint64_t blockId, const void * lpBuffer)
        将一个数据块写入到当前的块设备中。
        @param blockId 要写入数据的数据块。
        @param lpBuffer 存放要写入数据的数据块的缓冲区首地址指针。

    void MFSBufferedBlockDevice::Flush()
        强制将所有更改写入基础设备中。

    void MFSBufferedBlockDevice::Close()
        将所有更改写入基础设备中，然后关闭当前的设备。底层设备不会被关闭。

*/

class MFSBufferedBlockDevice
    : public MFSBlockDevice
{
public:
    MFSBufferedBlockDevice(MFSRawDevice * rawDevice, uint32_t bufferedBlockCount = 64);
    ~MFSBufferedBlockDevice() override;

    bool ReadBlock(void * lpBuffer, uint64_t blockId) override;
    bool WriteBlock(uint64_t blockId, const void * lpBuffer) override;

    void Flush();
    void Close() override;

private:
    std::unordered_map<uint64_t, MFSRawDeviceView *> _wreq;
    uint32_t _bufferedBlockCount;

    MFSRawDeviceView * LoadBlockIntoBuffer(uint64_t blockId);
};
