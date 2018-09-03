#pragma once

#include <cstdint>
#include <unordered_map>

#include "../../include/MFSString.h"
#include "../../include/fs/MFSMetas.h"

class MFSDirectoryBlock
{
public:
	class Iterator
	{
	private:
		using base_iter = typename std::unordered_map<MFSString, MFSFSDirectoryItem>::iterator;

		base_iter _iter;

	public:
		using difference_type = typename base_iter::difference_type;
		using value_type = typename base_iter::value_type;
		using pointer = typename base_iter::pointer;
		using reference = typename base_iter::reference;
		using iterator_category = typename base_iter::iterator_category;

		Iterator(base_iter);

		Iterator& operator ++ ();
		Iterator operator ++(int);
		bool operator == (const Iterator&) const;
		bool operator != (const Iterator&) const;
		value_type operator * () const;
	};

	Iterator begin() const;
	Iterator end() const;

	MFSDirectoryBlock(size_t size);
	~MFSDirectoryBlock();

	MFSFSDirectoryItem* FindDir(const MFSString& name) const;
	MFSFSDirectoryItem* AddDir(const MFSString& name);
	uint32_t EraseDir(const MFSString& name);

	bool Empty() const;

private:
	size_t _usedSize, _blockSize;
	mutable std::unordered_map<MFSString, MFSFSDirectoryItem> _dir;

	friend class MFSDirectoryBlockSerializer;
};
