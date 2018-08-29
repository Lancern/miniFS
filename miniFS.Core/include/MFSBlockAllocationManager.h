#pragma once

#include "MFSBitmap.h"

class MFSBlockAllocationManager
{
public:
	static constexpr uint32_t InvalidBlockId = ~0;

	MFSBlockAllocationManager(MFSBitmap* _bitmap);
	~MFSBlockAllocationManager();

	uint32_t AllocBlock();
	void FreeBlock(uint32_t pos);

private:
	MFSBitmap* _bitmap;
	uint32_t _alloc;
};
