#include "../../include/fs/MFSPartition.h"



#define CBLOCK_STREAM_MAX_LEN        0xFFFFFFFFFFFFFFFF


MFSPartition::ChainedBlockStream::ChainedBlockStream(MFSPartition * partition, uint32_t firstBlockId)
    : ChainedBlockStream(partition, firstBlockId, CBLOCK_STREAM_MAX_LEN)
{
    _length = GetLength();
}

MFSPartition::ChainedBlockStream::ChainedBlockStream(MFSPartition * partition, uint32_t firstBlockId, uint64_t length)
    : MFSBlockStream(partition->_device), _partition(partition), _firstBlock(firstBlockId),
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

uint64_t MFSPartition::ChainedBlockStream::GetLength() const
{
    if (_length != CBLOCK_STREAM_MAX_LEN)
        return _length;
    else
    {
        uint64_t result = 0;
        uint32_t blockSize = GetDeviceBlockSize();
        uint32_t blockId = _firstBlock;
        while (blockId != MFSFileAllocationTable::InvalidBlockId)
        {
            result += blockSize;
            blockId = GetNextBlockId(blockId);
        }

        return result;
    }
}

uint64_t MFSPartition::ChainedBlockStream::GetPosition() const
{
    uint64_t result = 0;
    uint32_t blockSize = GetDeviceBlockSize();
    uint32_t blockId = _firstBlock;

    while (blockId != _currentBlock)
    {
        result += blockSize;
        blockId = GetNextBlockId(blockId);
    }

    result += _blockOffset;
    return result;
}

bool MFSPartition::ChainedBlockStream::Seek(MFSStreamSeekOrigin origin, int64_t offset)
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

uint64_t MFSPartition::ChainedBlockStream::OnBlockSwap(uint64_t currentBlock)
{
    uint32_t nextBlock = GetNextBlockId(static_cast<uint32_t>(currentBlock));
    return nextBlock == MFSFileAllocationTable::InvalidBlockId
        ? _partition->_device->GetBlocksCount()
        : nextBlock;
}

uint32_t MFSPartition::ChainedBlockStream::GetNextBlockId(uint32_t current) const
{
    return _partition->_blockChain->Get(current);
}

uint32_t MFSPartition::ChainedBlockStream::GetNextBlockId() const
{
    return GetNextBlockId(_currentBlock);
}

bool MFSPartition::ChainedBlockStream::SeekBegin(int64_t offset)
{
    if (offset < 0)
        return false;

    uint32_t blockSize = GetDeviceBlockSize();
    uint32_t blockId = _firstBlock;
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
        _blockOffset = static_cast<uint32_t>(offset);
        return true;
    }
}

bool MFSPartition::ChainedBlockStream::SeekRelative(int64_t offset)
{
    return SeekBegin(GetPosition() + offset);
}

bool MFSPartition::ChainedBlockStream::SeekEnd(int64_t offset)
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