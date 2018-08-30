#include "..\..\include\fs\MFSDirectoryBlock.h"

MFSDirectoryBlock::MFSDirectoryBlock(size_t size)
	: _dictUsed(0), _heapUsed(0), _blockSize(size),
	_dictSize(size / 4 / sizeof(MFSFSDirectoryItem)), _heapSize((size - _dictSize) / sizeof(WCHAR)),
	_heapBase(new WCHAR[_heapSize]),
	_dict(0), _heapPool(_heapSize + 1)
{
}

MFSDirectoryBlock::~MFSDirectoryBlock()
{
	delete[] _heapBase;
}

MFSFSDirectoryItem * MFSDirectoryBlock::AddDict(const MFSString & name)
{
	if (_dict.size() == _dictSize) return nullptr;
	WCHAR* heapPos = HeapAlloc(name.GetLength() + 1);
	if (heapPos == nullptr) return nullptr;
	_dict.push_back({ heapPos - _heapBase, ~0 });
	_dictUsed++;
	for (size_t i = 0; name[i]; i++)
		*(heapPos++) = name[i];
	return &_dict.back();
}

void MFSDirectoryBlock::EraseDict(const MFSString & name)
{
	for (auto i = _dict.begin(); i != _dict.end(); i++)
	{
		if (i->nameOffset + name.GetLength() + 1 >= _heapSize) continue;
		if (*(_heapBase + i->nameOffset + name.GetLength())) continue;
		bool flag = true;
		for (size_t j = 0; name[j]; j++)
			if (name[j] != *(_heapBase + i->nameOffset + j))
			{
				flag = false;
				break;
			}
		if (flag)
		{
			HeapFree(_heapBase + i->nameOffset);
			_dict.erase(i);
			_dictUsed--;
			break;
		}
	}
}

MFSFSDirectoryItem * MFSDirectoryBlock::FindDict(const MFSString & name) const
{
	for (auto& it : _dict)
	{
		if (it.nameOffset + name.GetLength() + 1 >= _heapSize) continue;
		if (*(_heapBase + it.nameOffset + name.GetLength())) continue;
		bool flag = true;
		for (size_t j = 0; name[j]; j++)
			if (name[j] != *(_heapBase + it.nameOffset + j))
			{
				flag = false;
				break;
			}
		if (flag) return &it;
	}
	return nullptr;
}

WCHAR * MFSDirectoryBlock::HeapAlloc(size_t size)
{
	if (_heapSize - _heapUsed < size) return nullptr;
	WCHAR* ret = nullptr;
	for (size_t i = size; i < _heapPool.size(); i++)
		if (!_heapPool[i].empty())
		{
			ret = _heapPool[i].front();
			_heapPool[i].pop_front();
			if (i > size)
				_heapPool[i - size].push_back(ret + size);
		}
	if (ret == nullptr)
	{
		HeapCompress();
		ret = HeapAlloc(size);
	}
	else _heapUsed += size;
	return ret;
}

void MFSDirectoryBlock::HeapFree(WCHAR * ptr)
{
	size_t size = 0;
	while (*(ptr + size))
		*(ptr + size++) = 0;
	size++;
	_heapUsed -= size;
	for (size_t i = 1; i < _heapPool.size(); i++)
		for (auto j = _heapPool[i].begin(); j != _heapPool[i].end(); j++)
			if (*j == ptr + size)
			{
				size += i;
				_heapPool[i].erase(j);
			}
	_heapPool[size].push_back(ptr);
}

void MFSDirectoryBlock::HeapCompress()
{
	WCHAR* buffer = new WCHAR[_heapSize];
	memset(buffer, 0, sizeof(WCHAR) * _heapSize);
	WCHAR* curr = buffer;
	for (auto& i : _dict)
	{
		uint32_t offset = curr - buffer;
		for (WCHAR* j = _heapBase + i.nameOffset; *j; j++)
			*(curr++) = *j;
		curr++;
		i.nameOffset = offset;
	}
	for (size_t i = 0; i < _heapSize; i++)
		*(_heapBase + i) = *(buffer + i);
	delete[] buffer;
	for (size_t i = 1; i < _heapPool.size(); i++)
		_heapPool[i].clear();
	if (_heapUsed < _heapSize)
		_heapPool[_heapSize - _heapUsed].push_back(_heapBase + _heapSize - _heapUsed);
}
