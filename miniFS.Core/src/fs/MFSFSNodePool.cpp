#include "../../include/fs/MFSFSNodePool.h"
#include "../../include/fs/MFSFileAllocationTable.h"
#include "../../include/MFSDateTime.h"


MFSFSNodePool::MFSFSNodePool(uint32_t numberOfNodes)
    : _pool(new MFSFSEntryMeta[numberOfNodes]), _nodesCount(numberOfNodes), _alloc(0)
{
    memset(_pool.get(), 0, numberOfNodes * sizeof(MFSFSEntryMeta));
}

uint32_t MFSFSNodePool::GetAvailableFSNodeId()
{
    if (_alloc == InvalidFSNodeId)
        return _alloc;

    uint32_t result = _alloc;
    _alloc = LocateNextFreeNode();

    return result;
}

bool MFSFSNodePool::Allocate(uint32_t fsnodeId)
{
    bool first = (_pool[fsnodeId].common.refCount == 0);
    if (first)
        InitializeFSNode(fsnodeId);
    
    ++_pool[fsnodeId].common.refCount;
    return first;
}

bool MFSFSNodePool::Release(uint32_t fsnodeId)
{
    return --_pool[fsnodeId].common.refCount == 0;
}

MFSFSEntryMeta & MFSFSNodePool::operator[](uint32_t fsnodeId)
{
    return _pool[fsnodeId];
}

MFSFSEntryMeta MFSFSNodePool::operator[](uint32_t fsnodeId) const
{
    return _pool[fsnodeId];
}

MFSFSEntryMeta & MFSFSNodePool::Get(uint32_t fsnodeId)
{
    return this->operator[](fsnodeId);
}

MFSFSEntryMeta MFSFSNodePool::Get(uint32_t fsnodeId) const
{
    return this->operator[](fsnodeId);
}

uint32_t MFSFSNodePool::LocateNextFreeNode()
{
    for (uint32_t next = _alloc + 1;
        next != _alloc;
        next = (next + 1) % _nodesCount)
    {
        if (_pool[next].common.refCount == 0)
            return next;
    }
    return InvalidFSNodeId;
}

void MFSFSNodePool::InitializeFSNode(uint32_t fsnodeId)
{
    MFSFSEntryMeta & meta = _pool[fsnodeId];
    meta.common.flags = MFS_FSENTRY_ACCESS_READ | MFS_FSENTRY_ACCESS_WRITE | MFS_FSENTRY_ACCESS_EXECUTE;
    meta.common.firstBlockId = MFSFileAllocationTable::InvalidBlockId;
    
    uint64_t timestamp = MFSGetCurrentTimestamp();
    MFSGetInteger64Struct(&meta.common.creationTimestamp, timestamp);
    MFSGetInteger64Struct(&meta.common.lastAccessTimestamp, timestamp);
    MFSGetInteger64Struct(&meta.common.lastModTimestamp, timestamp);

    meta.common.refCount = 0;
    meta.spec.directoryMeta.childCount = 0;
}
