#include "../../include/fs/MFSBlockAllocationBitmap.h"

MFSBlockAllocationBitmap::Bitmap::Reference::Reference(uint64_t & value, uint32_t offset)
	: value(value), offset(offset)
{
}

MFSBlockAllocationBitmap::Bitmap::Reference::~Reference()
{
}

MFSBlockAllocationBitmap::Bitmap::Reference & 
	MFSBlockAllocationBitmap::Bitmap::Reference::operator=(bool x)
{
	if (x)
	{
		uint64_t mask = 1ull << offset;
		value |= mask;
	}
	else
	{
		uint64_t mask = ~(1ull << offset);
		value &= mask;
	}
	return *this;
}

MFSBlockAllocationBitmap::Bitmap::Reference & 
	MFSBlockAllocationBitmap::Bitmap::Reference::operator=(const Reference & x)
{
	*this = (bool)x;
	return *this;
}

MFSBlockAllocationBitmap::Bitmap::Reference::operator bool() const
{
	return value >> offset & 1;
}

MFSBlockAllocationBitmap::Bitmap::Bitmap(uint32_t sizeInBits)
	: _bitmap(sizeInBits / BIT_PACK_SIZE + (sizeInBits % BIT_PACK_SIZE ? 1 : 0))
{
}

MFSBlockAllocationBitmap::Bitmap::~Bitmap()
{
}

MFSBlockAllocationBitmap::Bitmap::Reference MFSBlockAllocationBitmap::Bitmap::operator[](uint32_t pos)
{
	uint32_t index = pos / BIT_PACK_SIZE, offset = pos % BIT_PACK_SIZE;
	return Reference(_bitmap[index], offset);
}

bool MFSBlockAllocationBitmap::Bitmap::operator[](uint32_t pos) const
{
	return Test(pos);
}

bool MFSBlockAllocationBitmap::Bitmap::Test(uint32_t pos) const
{
	uint32_t index = pos / BIT_PACK_SIZE, offset = pos % BIT_PACK_SIZE;
	return _bitmap[index] >> offset & 1;
}

uint32_t MFSBlockAllocationBitmap::Bitmap::Size() const
{
	return _bitmap.size() * BIT_PACK_SIZE;
}

void MFSBlockAllocationBitmap::Bitmap::Set(uint32_t pos, bool value)
{
	uint32_t index = pos / BIT_PACK_SIZE, offset = pos % BIT_PACK_SIZE;
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

void MFSBlockAllocationBitmap::Bitmap::Reset(uint32_t pos)
{
	uint32_t index = pos / BIT_PACK_SIZE, offset = pos % BIT_PACK_SIZE;
	uint64_t mask = ~(1ULL << offset);
	_bitmap[index] &= mask;
}

MFSBlockAllocationBitmap::MFSBlockAllocationBitmap(uint32_t sizeBits)
	: _bitmap(new Bitmap(sizeBits)), _alloc(0)
{
}

MFSBlockAllocationBitmap::~MFSBlockAllocationBitmap()
{
}

uint32_t MFSBlockAllocationBitmap::AllocBlock()
{
	if (_alloc == InvalidBlockId) 
        return InvalidBlockId;

	uint32_t ret = _alloc;
    Set(_alloc);

	return ret;
}

bool MFSBlockAllocationBitmap::AllocBlock(uint32_t blockId)
{
    if (_bitmap->Test(blockId))
        return false;

    _bitmap->Set(blockId);
    if (blockId == _alloc)
        LocateNextFreeBlock();

    return true;
}

bool MFSBlockAllocationBitmap::FreeBlock(uint32_t pos)
{
    bool result = _bitmap->Test(pos);
    Reset(pos);
    return result;
}

void MFSBlockAllocationBitmap::Set(uint32_t pos)
{
    _bitmap->Set(pos);
    if (pos == _alloc)
        LocateNextFreeBlock();
}

void MFSBlockAllocationBitmap::Reset(uint32_t pos)
{
    _bitmap->Reset(pos);
    if (_alloc == InvalidBlockId)
        _alloc = pos;
}

void MFSBlockAllocationBitmap::LocateNextFreeBlock()
{
    bool found = false;
    for (uint32_t i = _alloc + 1; i != _alloc; i = (i + 1) % _bitmap->Size())
    {
        if (!_bitmap->Test(i))
        {
            found = true;
            _alloc = i;
            break;
        }
    }

    if (!found)
        _alloc = InvalidBlockId;
}

