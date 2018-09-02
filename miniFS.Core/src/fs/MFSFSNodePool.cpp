#include "../../include/fs/MFSFSNodePool.h"


MFSFSNodePool::MFSFSNodePool(size_t numberOfNodes)
    : _pool(new MFSFSEntryMeta[numberOfNodes]), _nodesCount(numberOfNodes), _alloc(0)
{
    memset(_pool.get(), 0, numberOfNodes * sizeof(MFSFSEntryMeta));
}

uint32_t MFSFSNodePool::Allocate()
{
    if (_alloc == InvalidFSNodeId)
        return _alloc;

    uint32_t fsnodeId = _alloc;
    _alloc = LocateNextFreeNode();

    return fsnodeId;
}

bool MFSFSNodePool::Allocate(uint32_t fsnodeId)
{
    if (_pool[fsnodeId].common.refCount)
        return false;
    else
        return true;
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

size_t MFSFSNodePool::LocateNextFreeNode()
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
