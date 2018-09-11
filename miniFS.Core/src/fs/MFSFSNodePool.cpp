#include "../../include/fs/MFSFSNodePool.h"
#include "../../include/fs/MFSFileAllocationTable.h"
#include "../../include/MFSDateTime.h"



MFSFSNodePool::Iterator & MFSFSNodePool::Iterator::operator++()
{
    if (_offset < static_cast<int64_t>(_container->GetNodesCount()))
    {
        ++_offset;
        LocateNextUsedNode();
    }
    return *this;
}

MFSFSNodePool::Iterator MFSFSNodePool::Iterator::operator++(int)
{
    Iterator ret = *this;
    this->operator++();
    return ret;
}

MFSFSNodePool::Iterator & MFSFSNodePool::Iterator::operator--()
{
    if (_offset >= 0)
    {
        --_offset;
        LocatePreviousUsedNode();
    }
    return *this;
}

MFSFSNodePool::Iterator MFSFSNodePool::Iterator::operator--(int)
{
    Iterator ret = *this;
    this->operator--();
    return ret;
}

bool MFSFSNodePool::Iterator::operator==(const Iterator & another) const
{
    return this->_container == another._container && this->_offset == another._offset;
}


bool MFSFSNodePool::Iterator::operator!=(const Iterator & another) const
{
    return !this->operator==(another);
}

auto MFSFSNodePool::Iterator::operator*() const -> typename value_type
{
    return _container->Get(_offset);
}

auto MFSFSNodePool::Iterator::operator*() -> typename reference
{
    return _container->Get(_offset);
}

auto MFSFSNodePool::Iterator::operator->() -> typename pointer
{
    return &_container->Get(_offset);
}

void MFSFSNodePool::Iterator::LocateNextUsedNode()
{
    while (_offset < static_cast<int64_t>(_container->GetNodesCount()) && 
        _container->Get(static_cast<uint32_t>(_offset)).common.refCount == 0)
        ++_offset;
}

void MFSFSNodePool::Iterator::LocatePreviousUsedNode()
{
    while (_offset >= 0 && 
        _container->Get(static_cast<uint32_t>(_offset)).common.refCount == 0)
        --_offset;
}

MFSFSNodePool::Iterator::Iterator(MFSFSNodePool * container, int64_t offset)
    : _container(container), _offset(offset)
{
    LocateNextUsedNode();
}




MFSFSNodePool::MFSFSNodePool(uint32_t numberOfNodes)
    : _pool(new MFSFSEntryMeta[numberOfNodes]), _nodesCount(numberOfNodes), _alloc(0)
{
    memset(_pool.get(), 0, numberOfNodes * sizeof(MFSFSEntryMeta));
}

uint32_t MFSFSNodePool::GetNodesCount() const
{
    return _nodesCount;
}

uint32_t MFSFSNodePool::GetAvailableFSNodeId()
{
    return _alloc;
}

bool MFSFSNodePool::Allocate(uint32_t fsnodeId)
{
    bool first = (_pool[fsnodeId].common.refCount == 0);
    if (first) 
    {
        InitializeFSNode(fsnodeId);
        if (_alloc == fsnodeId)
            _alloc = LocateNextFreeNode();
    }
    
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

MFSFSNodePool::Iterator MFSFSNodePool::begin()
{
    return Iterator(this, 0);
}

MFSFSNodePool::Iterator MFSFSNodePool::end()
{
    return Iterator(this, static_cast<int64_t>(GetNodesCount()));
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
