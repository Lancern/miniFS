#include "../../include/fs/MFSPartition.h"
#include "../../include/device/MFSSparseBlockDevice.h"


MFSPartition::Internals::Internals(MFSPartition * host)
    : _partition(host)
{
}

MFSPartition * MFSPartition::Internals::GetPartition() const
{
    return _partition;
}

MFSFSMasterInfo MFSPartition::Internals::GetMasterInfo() const
{
    return _partition->_master;
}

MFSBlockAllocationBitmap * MFSPartition::Internals::GetBAB() const
{
    return _partition->_blockAllocation.get();
}

MFSFileAllocationTable * MFSPartition::Internals::GetFAT() const
{
    return _partition->_blockChain.get();
}

MFSFSNodePool * MFSPartition::Internals::GetFSNodePool() const
{
    return _partition->_fsnodePool.get();
}

uint32_t MFSPartition::Internals::GetNextAvailableDeviceBlockId() const
{
    return _partition->_blockAllocation->GetAvailableBlockId();
}

uint32_t MFSPartition::Internals::AllocateDeviceBlock(bool initialization)
{
    uint32_t blockId = _partition->_blockAllocation->AllocBlock();
    if (blockId != MFSBlockAllocationBitmap::InvalidBlockId)
    {
        _partition->_blockChain->Set(blockId, MFSFileAllocationTable::InvalidBlockId);

        if (initialization)
        {
            // Initialize the block to zero.
            InitializeDeviceBlock(blockId);
        }

        --_partition->_master.freeBlocks;
    }
    return blockId;
}

bool MFSPartition::Internals::AllocateDeviceBlock(uint32_t blockId, bool initialization)
{
    bool result = _partition->_blockAllocation->AllocBlock(blockId);
    if (result)
    {
        _partition->_blockChain->Set(blockId, MFSFileAllocationTable::InvalidBlockId);
        
        if (initialization)
        {
            // Initialize the block to zero.
            InitializeDeviceBlock(blockId);
        }

        --_partition->_master.freeBlocks;
    }
    return result;
}

void MFSPartition::Internals::InitializeDeviceBlock(uint32_t blockId)
{
    const void * buffer = VirtualAlloc(NULL, _partition->_device->GetBlockSize(), 
        MEM_RESERVE | MEM_COMMIT, PAGE_READONLY);
    _partition->_device->WriteBlock(blockId, buffer);
    VirtualFree(const_cast<void *>(buffer), 0, MEM_RELEASE);
}

bool MFSPartition::Internals::FreeDeviceBlock(uint32_t blockId)
{
    if (_partition->_blockAllocation->FreeBlock(blockId))
    {
        ++_partition->_master.freeBlocks;
    }
    return true;
}

uint32_t MFSPartition::Internals::AllocateTailBlock(uint32_t firstBlockId, bool initialization)
{
    uint32_t blockId = AllocateDeviceBlock(initialization);
    _partition->_blockChain->AddTail(firstBlockId, blockId);
    return blockId;
}

uint32_t MFSPartition::Internals::AllocateFrontBlock(uint32_t firstBlockId, bool initialization)
{
    uint32_t blockId = AllocateDeviceBlock(initialization);
    if (blockId == MFSBlockAllocationBitmap::InvalidBlockId)
        return MFSBlockAllocationBitmap::InvalidBlockId;

    _partition->_blockChain->AddFront(firstBlockId, blockId);
    return blockId;
}

uint32_t MFSPartition::Internals::AllocateBlockChain(uint32_t numberOfBlocks, bool initialization)
{
    if (numberOfBlocks == 0 || numberOfBlocks > _partition->_master.freeBlocks)
        return MFSBlockAllocationBitmap::InvalidBlockId;

    uint32_t firstBlockId = AllocateDeviceBlock(initialization);
    if (firstBlockId == MFSBlockAllocationBitmap::InvalidBlockId)
        return MFSBlockAllocationBitmap::InvalidBlockId;

    --numberOfBlocks;
    while (numberOfBlocks > 0)
    {
        uint32_t allocated = AllocateFrontBlock(firstBlockId, initialization);
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

uint32_t MFSPartition::Internals::FreeChainedBlock(uint32_t firstBlockId, uint32_t blockId)
{
    uint32_t first = _partition->_blockChain->Remove(firstBlockId, blockId);
    FreeDeviceBlock(blockId);
    return first;
}

uint32_t MFSPartition::Internals::FreeBlockAfter(uint32_t position)
{
    uint32_t block = _partition->_blockChain->RemoveAfter(position);
    if (block != MFSFileAllocationTable::InvalidBlockId)
        FreeDeviceBlock(block);
    return block;
}

uint32_t MFSPartition::Internals::AppendTailBlock(uint32_t firstBlockId, uint32_t blockId)
{
    if (firstBlockId == MFSFileAllocationTable::InvalidBlockId)
        return blockId;
    else
    {
        _partition->_blockChain->AddTail(firstBlockId, blockId);
        return firstBlockId;
    }
}

uint32_t MFSPartition::Internals::GetNextChainedBlock(uint32_t blockId) const
{
    return _partition->_blockChain->Get(blockId);
}

void MFSPartition::Internals::SetNextChainedBlock(uint32_t blockId, uint32_t nextBlockId)
{
    _partition->_blockChain->Set(blockId, nextBlockId);
}

uint32_t MFSPartition::Internals::GetAvailableFSNodeId()
{
    return _partition->_fsnodePool->GetAvailableFSNodeId();
}

bool MFSPartition::Internals::AllocateEntryMeta(uint32_t fsnodeId)
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

MFSBlockStream * MFSPartition::Internals::OpenBlockStream(uint32_t firstBlock)
{
    return new ChainedBlockStream(_partition, firstBlock);
}

MFSBlockStream * MFSPartition::Internals::OpenBlockStream(uint32_t firstBlock, uint64_t length)
{
    return new ChainedBlockStream(_partition, firstBlock, length);
}

bool MFSPartition::Internals::CopyBlock(uint32_t fromBlockId, uint32_t toBlockId)
{
    if (fromBlockId == toBlockId)
        return true;

    std::unique_ptr<uint8_t[]> buffer(new uint8_t[_partition->_device->GetBlockSize()]);
    if (!_partition->_device->ReadBlock(buffer.get(), fromBlockId))
        return false;
    if (!_partition->_device->WriteBlock(toBlockId, buffer.get()))
        return false;
    return true;
}
