#include "../include/MFSBitmap.h"

MFSBitmap::Reference::Reference(uint64_t& value, size_t offset)
	: value(value), offset(offset)
{
}

MFSBitmap::Reference::~Reference()
{
}

MFSBitmap::Reference & MFSBitmap::Reference::operator=(bool x)
{
	if (x)
	{
		uint64_t mask = 1ULL << offset;
		value |= mask;
	}
	else
	{
		uint64_t mask = ~(1ULL << offset);
		value &= mask;
	}
	return *this;
}

MFSBitmap::Reference & MFSBitmap::Reference::operator=(const Reference & x)
{
	*this = (bool)x;
	return *this;
}

MFSBitmap::Reference::operator bool() const
{
	return value >> offset & 1;
}

MFSBitmap::MFSBitmap(size_t size)
	: _bitmap(size / BIT_PACK_SIZE)
{
}

MFSBitmap::~MFSBitmap()
{
}

MFSBitmap::Reference MFSBitmap::operator[](size_t pos)
{
	size_t index = pos / BIT_PACK_SIZE, offset = pos % BIT_PACK_SIZE;
	return Reference(_bitmap[index], offset);
}

bool MFSBitmap::operator[](size_t pos) const
{
	return Test(pos);
}

bool MFSBitmap::Test(size_t pos) const
{
	size_t index = pos / BIT_PACK_SIZE, offset = pos % BIT_PACK_SIZE;
	return _bitmap[index] >> offset & 1;
}

size_t MFSBitmap::Size() const
{
	return _bitmap.size() * BIT_PACK_SIZE;
}

void MFSBitmap::Set(size_t pos, bool value)
{
	size_t index = pos / BIT_PACK_SIZE, offset = pos % BIT_PACK_SIZE;
	if (value)
	{
		uint64_t mask = 1ULL << offset;
		_bitmap[index] |= mask;
	}
	else
	{
		uint64_t mask = ~(1ULL << offset);
		_bitmap[index] &= mask;
	}
}

void MFSBitmap::Reset(size_t pos)
{
	size_t index = pos / BIT_PACK_SIZE, offset = pos % BIT_PACK_SIZE;
	uint64_t mask = ~(1ULL << offset);
	_bitmap[index] &= mask;
}
