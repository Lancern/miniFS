#pragma once

#include "MFSStream.h"
#include "../device/MFSBlockDevice.h"
#include <memory>


/*

class MFSBlockStream
表示以一个块设备作为基础设备的流对象。

    成员函数：

    bool MFSBlockStream::SeekBlock(UINT64 blockId)
        移动流指针到指定设备块的起始位置。

    UINT64 MFSBlockStream::GetCurrentBlockId() const
        获取当前流指针所位于的块编号。

    UINT64 MFSBlockStream::GetBlockInternalOffset() const
        获取当前流指针所位于的块内偏移量。

    bool MFSBlockStream::IsDirty() const
        获取一个值指示是否在当前活动块上进行过写操作。

    void MFSBlockStream::SetDirtyFlag(bool dirty)
        设置当前活动块的脏标记。

    UINT64 MFSBlockStream::OnBlockSwap(UINT64 currentBlock)
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

    UINT64 GetLength() const override;
    UINT64 GetPosition() const override;

    UINT64 GetCurrentBlockId() const;
    UINT64 GetBlockInternalOffset() const;

    MFSBlockDevice * GetDevice() const;
    DWORD GetDeviceBlockSize() const;
    UINT64 GetDeviceBlocksCount() const;

    DWORD Read(LPVOID lpBuffer, DWORD dwBufferSize, DWORD dwNumberOfBytesToRead) override;
    DWORD Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite) override;
    bool Seek(MFSStreamSeekOrigin origin, INT64 offset) override;

    void Flush() override;
    void Close() override;

protected:
    bool SeekBlock(UINT64 blockId);

    bool IsDirty() const;
    void SetDirtyFlag(bool dirty);

    virtual UINT64 OnBlockSwap(UINT64 currentBlock);

private:
    MFSBlockDevice * _device;
    std::unique_ptr<BYTE[]> _buffer;
    DWORD _insideOffset;
    UINT64 _blockOffset;
    bool _dirty;

    bool SeekFromBegin(INT64 offset);

    bool TryReadByte(BYTE * buffer);
    bool TryWriteByte(BYTE data);
};
