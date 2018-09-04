#include "../../include/fs/MFSPartition.h"



#define CBLOCK_STREAM_MAX_LEN        0xFFFFFFFFFFFFFFFF


MFSPartition::ChainedBlockStream::ChainedBlockStream(MFSPartition * partition, DWORD firstBlockId)
    : ChainedBlockStream(partition, firstBlockId, CBLOCK_STREAM_MAX_LEN)
{
    _length = GetLength();
}

MFSPartition::ChainedBlockStream::ChainedBlockStream(MFSPartition * partition, DWORD firstBlockId, UINT64 length)
    : MFSBlockStream(partition->_device.get()), _partition(partition), _firstBlock(firstBlockId),
    _currentBlock(firstBlockId), _blockOffset(0), _length(length)
{
    MFSBlockStream::Seek(MFSStreamSeekOrigin::Begin, GetDeviceBlockSize() * _firstBlock);
}

MFSPartition * MFSPartition::ChainedBlockStream::GetPartition() const
{
    return _partition;
}

bool MFSPartition::ChainedBlockStream::HasNext() const
{
    return _currentBlock != MFSFileAllocationTable::InvalidBlockId;
}

UINT64 MFSPartition::ChainedBlockStream::GetLength() const
{
    if (_length != CBLOCK_STREAM_MAX_LEN)
        return _length;
    else
    {
        UINT64 result = 0;
        DWORD blockSize = GetDeviceBlockSize();
        DWORD blockId = _firstBlock;
        while (blockId != MFSFileAllocationTable::InvalidBlockId)
        {
            result += blockSize;
            blockId = GetNextBlockId(blockId);
        }

        return result;
    }
}

UINT64 MFSPartition::ChainedBlockStream::GetPosition() const
{
    UINT64 result = 0;
    DWORD blockSize = GetDeviceBlockSize();
    DWORD blockId = _firstBlock;

    while (blockId != _currentBlock)
    {
        result += blockSize;
        blockId = GetNextBlockId(blockId);
    }

    result += _blockOffset;
    return result;
}

bool MFSPartition::ChainedBlockStream::Seek(MFSStreamSeekOrigin origin, INT64 offset)
{
    switch (origin)
    {
    case MFSStreamSeekOrigin::Begin:
        return SeekBegin(offset);
    case MFSStreamSeekOrigin::Relative:
        return SeekRelative(offset);
    case MFSStreamSeekOrigin::End:
        return SeekEnd(offset);
    default:
        return false;
    }
}

UINT64 MFSPartition::ChainedBlockStream::OnBlockSwap(UINT64 currentBlock)
{
    DWORD nextBlock = GetNextBlockId(static_cast<DWORD>(currentBlock));
    return nextBlock == MFSFileAllocationTable::InvalidBlockId
        ? _partition->_device->GetBlocksCount()
        : nextBlock;
}

DWORD MFSPartition::ChainedBlockStream::GetNextBlockId(DWORD current) const
{
    return _partition->_blockChain->Get(current);
}

DWORD MFSPartition::ChainedBlockStream::GetNextBlockId() const
{
    return GetNextBlockId(_currentBlock);
}

bool MFSPartition::ChainedBlockStream::SeekBegin(INT64 offset)
{
    if (offset < 0)
        return false;

    DWORD blockSize = GetDeviceBlockSize();
    DWORD blockId = _firstBlock;
    while (offset >= blockSize && blockId != MFSFileAllocationTable::InvalidBlockId)
    {
        offset -= blockSize;
        blockId = GetNextBlockId(blockId);
    }

    if (blockId == MFSFileAllocationTable::InvalidBlockId)
    {
        if (offset != 0)
            return false;
        else
        {
            _currentBlock = MFSFileAllocationTable::InvalidBlockId;
            _blockOffset = 0;
            return true;
        }
    }
    else
    {
        _currentBlock = blockId;
        _blockOffset = static_cast<DWORD>(offset);
        return true;
    }
}

bool MFSPartition::ChainedBlockStream::SeekRelative(INT64 offset)
{
    return SeekBegin(GetPosition() + offset);
}

bool MFSPartition::ChainedBlockStream::SeekEnd(INT64 offset)
{
    if (offset > 0)
        return false;
    else if (offset == 0)
    {
        _currentBlock = MFSFileAllocationTable::InvalidBlockId;
        _blockOffset = 0;
        return true;
    }
    else
    {
        return SeekBegin(GetLength() + offset);
    }
}