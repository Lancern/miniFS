#include "../../include/fs/MFSAllocationTable.h"

MFSAllocationTable::MFSAllocationTable(size_t size)
	: _next(size)
{
}

MFSAllocationTable::~MFSAllocationTable()
{
}

uint32_t & MFSAllocationTable::operator[](size_t pos)
{
	return _next[pos];
}

uint32_t MFSAllocationTable::operator[](size_t pos) const
{
	return _next[pos];
}

void MFSAllocationTable::Add(uint32_t first, uint32_t last)
{
	_next[first] = last;
}

size_t MFSAllocationTable::Size() const
{
	return _next.size();
}
