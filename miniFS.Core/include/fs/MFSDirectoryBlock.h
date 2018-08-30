#pragma once

#include <cstdint>
#include <vector>
#include <list>

#include "../../include/MFSString.h"
#include "../../include/fs/MFSMetas.h"

class MFSDirectoryBlock
{
public:
	MFSDirectoryBlock(size_t size);
	~MFSDirectoryBlock();

	MFSFSDirectoryItem* AddDict(const MFSString& name);
	void EraseDict(const MFSString& name);
	MFSFSDirectoryItem* FindDict(const MFSString& name) const;

private:
	uint32_t _dictUsed, _heapUsed;
	size_t _blockSize, _dictSize, _heapSize;
	WCHAR* _heapBase;
	mutable std::list<MFSFSDirectoryItem> _dict;
	std::vector<std::list<WCHAR*>> _heapPool;

	WCHAR* HeapAlloc(size_t size);
	void HeapFree(WCHAR* ptr);
	void HeapCompress();

	friend class MFSDirectoryBlockSerializer;
};
