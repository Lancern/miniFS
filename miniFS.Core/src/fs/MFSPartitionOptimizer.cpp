#include "../../include/fs/MFSPartition.h"


MFSPartitionOptimizer::MFSPartitionOptimizer(MFSPartition::Internals partition)
    : _partition(partition)
{
}

void MFSPartitionOptimizer::Optimize()
{
    for (MFSFSEntryMeta & meta : *_partition.GetFSNodePool())
    {
        if (meta.common.firstBlockId == MFSFileAllocationTable::InvalidBlockId)
            continue;

        meta.common.firstBlockId = OptimizeBlock(meta.common.firstBlockId);
        for (uint32_t baseBlock = meta.common.firstBlockId;
            baseBlock != MFSFileAllocationTable::InvalidBlockId;
            baseBlock = _partition.GetNextChainedBlock(baseBlock))
        {
            uint32_t nextBlock = _partition.GetNextChainedBlock(baseBlock);
            _partition.SetNextChainedBlock(baseBlock, OptimizeBlock(nextBlock));
        }
    }
}

uint32_t MFSPartitionOptimizer::OptimizeBlock(uint32_t blockId)
{
    uint32_t targetBlockId = _partition.GetAvailableFSNodeId();
    if (targetBlockId != MFSBlockAllocationBitmap::InvalidBlockId &&
        targetBlockId < blockId)
    {
        if (!_partition.AllocateDeviceBlock(targetBlockId))
            return blockId;
        if (!_partition.CopyBlock(blockId, targetBlockId))
        {
            _partition.FreeDeviceBlock(blockId);
            return blockId;
        }

        uint32_t chainedNextBlock = _partition.GetNextChainedBlock(blockId);
        _partition.SetNextChainedBlock(targetBlockId, chainedNextBlock);

        _partition.FreeDeviceBlock(blockId);
        return targetBlockId;
    }
    else
    {
        return blockId;
    }
}
