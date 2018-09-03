#include "../../include/fs/MFSFileAllocationTable.h"


MFSFileAllocationTable::MFSFileAllocationTable(uint32_t size)
	: _next(size)
{
}

MFSFileAllocationTable::~MFSFileAllocationTable()
{
}

uint32_t & MFSFileAllocationTable::operator[](uint32_t pos)
{
	return _next[pos];
}

uint32_t MFSFileAllocationTable::operator[](uint32_t pos) const
{
	return _next[pos];
}

uint32_t MFSFileAllocationTable::Get(uint32_t pos) const
{
    return (*this)[pos];
}

void MFSFileAllocationTable::Set(uint32_t pos, uint32_t value)
{
    _next[pos] = value;
}

uint32_t MFSFileAllocationTable::GetCount() const
{
    return static_cast<uint32_t>(_next.size());
}

uint32_t MFSFileAllocationTable::GetTailBlock(uint32_t first) const
{
    if (first == InvalidBlockId)
        return first;
    else
    {
        while (_next[first] != InvalidBlockId)
            first = _next[first];
        return first;
    }
}

uint32_t MFSFileAllocationTable::GetBlockBefore(uint32_t first, uint32_t blockId) const
{
    if (first == InvalidBlockId)
        return first;

    while (first != InvalidBlockId && _next[first] != blockId)
        first = _next[first];

    return first;
}

void MFSFileAllocationTable::AddAfter(uint32_t position, uint32_t blockId)
{
    _next[blockId] = _next[position];
    _next[position] = blockId;
}

void MFSFileAllocationTable::AddTail(uint32_t first, uint32_t blockId)
{
    AddAfter(GetTailBlock(first), blockId);
}

void MFSFileAllocationTable::AddFront(uint32_t first, uint32_t blockId)
{
    _next[blockId] = first;
}

uint32_t MFSFileAllocationTable::RemoveAfter(uint32_t position)
{
    uint32_t next = _next[position];
    if (next != InvalidBlockId)
        _next[position] = _next[next];
    return next;
}

uint32_t MFSFileAllocationTable::Remove(uint32_t first, uint32_t blockId)
{
    if (first == blockId)
    {
        uint32_t second = _next[first];
        _next[first] = InvalidBlockId;
        return second;
    }
    else
    {
        uint32_t before = GetBlockBefore(first, blockId);
        if (before != InvalidBlockId)
            RemoveAfter(before);
        return first;
    }
}

uint32_t MFSFileAllocationTable::RemoveFront(uint32_t first)
{
    uint32_t next = _next[first];
    _next[first] = InvalidBlockId;
    return next;
}
