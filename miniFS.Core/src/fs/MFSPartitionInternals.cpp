#include "../../include/fs/MFSPartition.h"


MFSPartition::Internals::Internals(MFSPartition * host)
    : _partition(host)
{
}

MFSPartition * MFSPartition::Internals::GetPartition() const
{
    return _partition;
}

DWORD MFSPartition::Internals::AllocateDeviceBlock()
{
    DWORD blockId = _partition->_blockAllocation->AllocBlock();
    if (blockId != MFSBlockAllocationBitmap::InvalidBlockId)
        _partition->_blockChain->Set(blockId, MFSFileAllocationTable::InvalidBlockId);
    return blockId;
}

bool MFSPartition::Internals::AllocateDeviceBlock(DWORD blockId)
{
    bool result = _partition->_blockAllocation->AllocBlock(blockId);
    if (result)
        _partition->_blockChain->Set(blockId, MFSFileAllocationTable::InvalidBlockId);
    return result;
}

bool MFSPartition::Internals::FreeDeviceBlock(DWORD blockId)
{
    _partition->_blockAllocation->FreeBlock(blockId);
    return true;
}

DWORD MFSPartition::Internals::AllocateTailBlock(DWORD firstBlockId)
{
    DWORD blockId = AllocateDeviceBlock();
    _partition->_blockChain->AddTail(firstBlockId, blockId);
    return blockId;
}

DWORD MFSPartition::Internals::AllocateFrontBlock(DWORD firstBlockId)
{
    DWORD blockId = AllocateDeviceBlock();
    if (blockId == MFSBlockAllocationBitmap::InvalidBlockId)
        return MFSBlockAllocationBitmap::InvalidBlockId;

    _partition->_blockChain->AddFront(firstBlockId, blockId);
    return blockId;
}

DWORD MFSPartition::Internals::AllocateBlockChain(DWORD numberOfBlocks)
{
    if (numberOfBlocks == 0)
        return MFSBlockAllocationBitmap::InvalidBlockId;

    uint32_t firstBlockId = AllocateDeviceBlock();
    if (firstBlockId == MFSBlockAllocationBitmap::InvalidBlockId)
        return firstBlockId;

    --numberOfBlocks;
    while (numberOfBlocks > 0)
    {
        uint32_t allocated = AllocateFrontBlock(firstBlockId);
        if (allocated == MFSBlockAllocationBitmap::InvalidBlockId)
        {
            while (firstBlockId != MFSFileAllocationTable::InvalidBlockId)
            {
                firstBlockId = FreeChainedBlock(firstBlockId, firstBlockId);
            }
            return MFSBlockAllocationBitmap::InvalidBlockId;
        }

        firstBlockId = allocated;
        --numberOfBlocks;
    }

    return firstBlockId;
}

DWORD MFSPartition::Internals::FreeChainedBlock(DWORD firstBlockId, DWORD blockId)
{
    DWORD first = _partition->_blockChain->Remove(firstBlockId, blockId);
    FreeDeviceBlock(blockId);
    return first;
}

DWORD MFSPartition::Internals::FreeBlockAfter(DWORD position)
{
    DWORD block = _partition->_blockChain->RemoveAfter(position);
    if (block != MFSFileAllocationTable::InvalidBlockId)
        FreeDeviceBlock(block);
    return block;
}

DWORD MFSPartition::Internals::AppendTailBlock(DWORD firstBlockId, DWORD blockId)
{
    if (firstBlockId == MFSFileAllocationTable::InvalidBlockId)
        return blockId;
    else
    {
        _partition->_blockChain->AddTail(firstBlockId, blockId);
        return firstBlockId;
    }
}

DWORD MFSPartition::Internals::GetNextChainedBlock(DWORD blockId) const
{
    return _partition->_blockChain->Get(blockId);
}

DWORD MFSPartition::Internals::GetAvailableFSNodeId()
{
    return _partition->_fsnodePool->GetAvailableFSNodeId();
}

bool MFSPartition::Internals::AllocateEntryMeta(DWORD fsnodeId)
{
    return _partition->_fsnodePool->Allocate(fsnodeId);
}

MFSFSEntryMeta * MFSPartition::Internals::GetEntryMeta(uint32_t fsnodeId) const
{
    return &_partition->_fsnodePool->Get(fsnodeId);
}

bool MFSPartition::Internals::FreeEntryMeta(uint32_t fsnodeId)
{
    uint32_t firstBlockId = _partition->_fsnodePool->Get(fsnodeId).common.firstBlockId;
    if (!_partition->_fsnodePool->Release(fsnodeId))
        return false;
    else
    {
        // Release block chain.
        while (firstBlockId != MFSFileAllocationTable::InvalidBlockId)
            firstBlockId = FreeChainedBlock(firstBlockId, firstBlockId);
        return true;
    }
}

MFSBlockStream * MFSPartition::Internals::OpenBlockStream(DWORD firstBlock)
{
    return new ChainedBlockStream(_partition, firstBlock);
}

MFSBlockStream * MFSPartition::Internals::OpenBlockStream(DWORD firstBlock, UINT64 length)
{
    return new ChainedBlockStream(_partition, firstBlock, length);
}
