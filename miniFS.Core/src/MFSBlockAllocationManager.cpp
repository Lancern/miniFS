#include "../include/MFSBlockAllocationManager.h"

MFSBlockAllocationManager::MFSBlockAllocationManager(MFSBitmap* _bitmap)
	: _bitmap(_bitmap), _alloc(0)
{
}

MFSBlockAllocationManager::~MFSBlockAllocationManager()
{
}

uint32_t MFSBlockAllocationManager::AllocBlock()
{
	if (_alloc == InvalidBlockId) return InvalidBlockId;
	uint32_t ret = _alloc;
	_bitmap->Set(_alloc);
	for (uint32_t i = _alloc + 1; i != _alloc; i = (i + 1) % _bitmap->Size())
		if (_bitmap->Test(i))
		{
			_alloc = i;
			break;
		}
	if (_alloc == ret) _alloc = InvalidBlockId;
	return ret;
}

void MFSBlockAllocationManager::FreeBlock(uint32_t pos)
{
	_bitmap->Reset(pos);
	if (_alloc == InvalidBlockId) _alloc = pos;
}

