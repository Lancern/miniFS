#include "../include/MFSAllocationTable.h"

MFSAllocationTable::MFSAllocationTable(size_t size)
	: _next(size)
{
}

MFSAllocationTable::~MFSAllocationTable()
{
}

int32_t & MFSAllocationTable::operator[](size_t pos)
{
	return _next[pos];
}

int32_t MFSAllocationTable::operator[](size_t pos) const
{
	return _next[pos];
}

void MFSAllocationTable::Add(int32_t first, int32_t last)
{
	_next[first] = last;
}

size_t MFSAllocationTable::Size() const
{
	return _next.size();
}
