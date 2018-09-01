#include "../../include/fs/MFSFileAllocationTable.h"

MFSFileAllocationTable::MFSFileAllocationTable(size_t size)
	: _next(size)
{
}

MFSFileAllocationTable::~MFSFileAllocationTable()
{
}

uint32_t & MFSFileAllocationTable::operator[](size_t pos)
{
	return _next[pos];
}

uint32_t MFSFileAllocationTable::operator[](size_t pos) const
{
	return _next[pos];
}

void MFSFileAllocationTable::Add(uint32_t first, uint32_t last)
{
	_next[first] = last;
}

size_t MFSFileAllocationTable::Size() const
{
	return _next.size();
}

uint32_t MFSFileAllocationTable::Get(size_t pos) const
{
    return (*this)[pos];
}

void MFSFileAllocationTable::Set(size_t pos, size_t value)
{
    _next[pos] = value;
}
