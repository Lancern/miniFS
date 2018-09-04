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
    return blockId;
}

bool MFSPartition::Internals::AllocateDeviceBlock(DWORD blockId)
{
    return _partition->_blockAllocation->AllocBlock(blockId);
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
    _partition->_blockChain->AddFront(firstBlockId, blockId);
    return blockId;
}

DWORD MFSPartition::Internals::FreeChainedBlock(DWORD firstBlockId, DWORD blockId)
{
    return _partition->_blockChain->Remove(firstBlockId, blockId);
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

MFSStream * MFSPartition::Internals::OpenBlockStream(DWORD firstBlock)
{
    return new ChainedBlockStream(_partition, firstBlock);
}

MFSStream * MFSPartition::Internals::OpenBlockStream(DWORD firstBlock, UINT64 length)
{
    return new ChainedBlockStream(_partition, firstBlock, length);
}
