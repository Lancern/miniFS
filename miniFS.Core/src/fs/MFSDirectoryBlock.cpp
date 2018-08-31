#include "..\..\include\fs\MFSDirectoryBlock.h"

MFSDirectoryBlock::MFSDirectoryBlock(size_t size)
	: _usedSize(4), _blockSize(size), _dir(0)
{
}

MFSDirectoryBlock::~MFSDirectoryBlock()
{
}

MFSFSDirectoryItem * MFSDirectoryBlock::FindDir(const MFSString & name) const
{
	if (_dir.find(name) == _dir.end()) return nullptr;
	return &_dir[name];
}

MFSFSDirectoryItem * MFSDirectoryBlock::AddDir(const MFSString & name)
{
	if (FindDir(name) != nullptr) return nullptr;
	size_t size = sizeof(MFSFSDirectoryItem) + (name.GetLength() + 1) * sizeof(WCHAR);
	if (_usedSize + size > _blockSize) return nullptr;
	_usedSize += size;
	_dir[name] = MFSFSDirectoryItem();
	return &_dir[name];
}

bool MFSDirectoryBlock::EraseDir(const MFSString & name)
{
	if (FindDir(name) == nullptr) return false;
	_dir.erase(name);
	size_t size = sizeof(MFSFSDirectoryItem) + (name.GetLength() + 1) * sizeof(WCHAR);
	_usedSize -= size;
	return true;
}
