#include "../include/MFSBitmap.h"

MFSBitmap::Reference::Reference(std::vector<bool>::reference value)
	: value(value)
{
}

MFSBitmap::Reference::~Reference()
{
}

MFSBitmap::Reference & MFSBitmap::Reference::operator=(bool x)
{
	value = x;
	return *this;
}

MFSBitmap::Reference & MFSBitmap::Reference::operator=(const Reference & x)
{
	value = x;
	return *this;
}

MFSBitmap::Reference::operator bool() const
{
	return value;
}

MFSBitmap::MFSBitmap(size_t size)
	: _bitmap(size)
{
}

MFSBitmap::~MFSBitmap()
{
}

MFSBitmap::Reference MFSBitmap::operator[](size_t pos)
{
	return _bitmap[pos];
}

bool MFSBitmap::operator[](size_t pos) const
{
	return _bitmap[pos];
}

bool MFSBitmap::Test(size_t pos) const
{
	return _bitmap[pos];
}

size_t MFSBitmap::Size() const
{
	return _bitmap.size();
}

void MFSBitmap::Set(size_t pos, bool value)
{
	_bitmap[pos] = value;
}

void MFSBitmap::Reset(size_t pos)
{
	_bitmap[pos] = false;
}

void MFSBitmap::Read(std::vector<void*>& blocks, size_t blockSize)
{
	for (size_t i = 0; i < blocks.size(); i++)
	{
		uint64_t* pValue = reinterpret_cast<uint64_t*>(blocks[i]);
		for (int j = 0; j < blockSize / 8; j++)
		{
			uint64_t x = *(pValue++);
			for (int k = 0; k < 64; k++)
				_bitmap[(i << 16) | (j << 6) | k] = x >> k & 1;
		}
	}
}

void MFSBitmap::Write(std::vector<void*>& blocks, size_t blockSize) const
{
	for (size_t i = 0; i < blocks.size(); i++)
	{
		uint64_t* pValue = reinterpret_cast<uint64_t*>(blocks[i]);
		for (int j = 0; j < blockSize / 8; j++)
		{
			uint64_t x = 0;
			for (int k = 0; k < 64; k++)
				x = x << 1 | _bitmap[(i << 16) | (j << 6) | k];
			*(pValue++) = x;
		}
	}
}
