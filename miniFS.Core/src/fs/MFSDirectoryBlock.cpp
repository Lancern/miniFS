#include "..\..\include\fs\MFSDirectoryBlock.h"

MFSDirectoryBlock::Iterator::Iterator(base_iter iter)
	: _iter(iter)
{
}

MFSDirectoryBlock::Iterator& MFSDirectoryBlock::Iterator::operator ++ ()
{
	++_iter;
	return *this;
}

MFSDirectoryBlock::Iterator MFSDirectoryBlock::Iterator::operator ++ (int)
{
	auto ret = *this;
	++_iter;
	return ret;
}

bool MFSDirectoryBlock::Iterator::operator == (const Iterator& rhs) const
{
	return _iter == rhs._iter;
}

bool MFSDirectoryBlock::Iterator::operator != (const Iterator& rhs) const
{
	return _iter != rhs._iter;
}

auto MFSDirectoryBlock::Iterator::operator * () const -> typename value_type
{
	return *_iter;
}

MFSDirectoryBlock::Iterator MFSDirectoryBlock::begin() const
{
	return Iterator(_dir.begin());
}

MFSDirectoryBlock::Iterator MFSDirectoryBlock::end() const
{
	return Iterator(_dir.end());
}

MFSDirectoryBlock::MFSDirectoryBlock(size_t size)
	: _usedSize(4), _blockSize(size), _dir(0)
{
}

MFSDirectoryBlock::~MFSDirectoryBlock()
{
}

MFSFSDirectoryItem * MFSDirectoryBlock::FindDir(const MFSString & name) const
{
	if (_dir.find(name) == _dir.end()) 
        return nullptr;
	return &_dir[name];
}

MFSFSDirectoryItem * MFSDirectoryBlock::AddDir(const MFSString & name)
{
	if (FindDir(name) != nullptr) 
        return nullptr;

	size_t size = sizeof(MFSFSDirectoryItem) + (name.GetLength() + 1) * sizeof(WCHAR);
	if (_usedSize + size > _blockSize) 
        return nullptr;
    
	_usedSize += size;
	_dir[name] = MFSFSDirectoryItem();
	return &_dir[name];
}

bool MFSDirectoryBlock::EraseDir(const MFSString & name)
{
	if (FindDir(name) == nullptr) 
        return false;

	_dir.erase(name);
	size_t size = sizeof(MFSFSDirectoryItem) + (name.GetLength() + 1) * sizeof(WCHAR);
	_usedSize -= size;
	return true;
}
