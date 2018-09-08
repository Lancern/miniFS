#pragma once

#include "MFSStream.h"
#include "../device/MFSBlockDevice.h"
#include <memory>


/*

class MFSBlockStream
��ʾ��һ�����豸��Ϊ�����豸��������

    ��Ա������

    bool MFSBlockStream::SeekBlock(uint64_t blockId)
        �ƶ���ָ�뵽ָ���豸�����ʼλ�á�

    uint64_t MFSBlockStream::GetCurrentBlockId() const
        ��ȡ��ǰ��ָ����λ�ڵĿ��š�

    uint64_t MFSBlockStream::GetBlockInternalOffset() const
        ��ȡ��ǰ��ָ����λ�ڵĿ���ƫ������

    bool MFSBlockStream::IsDirty() const
        ��ȡһ��ֵָʾ�Ƿ��ڵ�ǰ����Ͻ��й�д������

    void MFSBlockStream::SetDirtyFlag(bool dirty)
        ���õ�ǰ�������ǡ�

    uint64_t MFSBlockStream::OnBlockSwap(uint64_t currentBlock)
        �������齻��ʱ�����ú�����������������дʱ�����ݵ�ǰ����ȷ��Ҫ����Ŀ��š�

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
