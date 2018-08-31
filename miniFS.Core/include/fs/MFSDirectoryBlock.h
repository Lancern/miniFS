#pragma once

#include <cstdint>
#include <unordered_map>

#include "../../include/MFSString.h"
#include "../../include/fs/MFSMetas.h"

class MFSDirectoryBlock
{
public:
	MFSDirectoryBlock(size_t size);
	~MFSDirectoryBlock();

	MFSFSDirectoryItem* FindDir(const MFSString& name) const;
	MFSFSDirectoryItem* AddDir(const MFSString& name);
	bool EraseDir(const MFSString& name);

private:
	size_t _usedSize, _blockSize;
	mutable std::unordered_map<MFSString, MFSFSDirectoryItem> _dir;

	friend class MFSDirectoryBlockSerializer;
};
