#pragma once

#include "MFSStream.h"
#include "../device/MFSBlockDevice.h"
#include <memory>


/*

class MFSBlockStream
表示以一个块设备作为基础设备的流对象。

    成员函数：

    bool MFSBlockStream::SeekBlock(uint64_t blockId)
        移动流指针到指定设备块的起始位置。

    uint64_t MFSBlockStream::GetCurrentBlockId() const
        获取当前流指针所位于的块编号。

    uint64_t MFSBlockStream::GetBlockInternalOffset() const
        获取当前流指针所位于的块内偏移量。

    bool MFSBlockStream::IsDirty() const
        获取一个值指示是否在当前活动块上进行过写操作。

    void MFSBlockStream::SetDirtyFlag(bool dirty)
        设置当前活动块的脏标记。

    uint64_t MFSBlockStream::OnBlockSwap(uint64_t currentBlock)
        当发生块交换时触发该函数。当在子类中重写时，根据当前块编号确定要换入的块编号。

*/

class MFSBlockStream
    : public MFSStream
{
public:
    MFSBlockStream(MFSBlockDevice * device);
    
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    bool HasNext() const override;

    uint64_t GetLength() const override;
    uint64_t GetPosition() const override;

    uint64_t GetCurrentBlockId() const;
    uint32_t GetBlockInternalOffset() const;

    MFSBlockDevice * GetDevice() const;
    uint32_t GetDeviceBlockSize() const;
    uint64_t GetDeviceBlocksCount() const;

    uint32_t Read(void * lpBuffer, uint32_t dwBufferSize, uint32_t dwNumberOfBytesToRead) override;
    uint32_t Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite) override;
    bool Seek(MFSStreamSeekOrigin origin, int64_t offset) override;

    void Flush() override;
    void Close() override;

protected:
    bool SeekBlock(uint64_t blockId);
    void SetBlockInternalOffset(uint32_t offset);

    bool IsDirty() const;
    void SetDirtyFlag(bool dirty);

    virtual uint64_t OnBlockSwap(uint64_t currentBlock);

private:
    MFSBlockDevice * _device;
    std::unique_ptr<uint8_t[]> _buffer;
    uint32_t _insideOffset;
    uint64_t _blockOffset;
    bool _dirty;

    bool SeekFromBegin(int64_t offset);

    bool TryReadByte(uint8_t * buffer);
    bool TryWriteByte(uint8_t data);
};
