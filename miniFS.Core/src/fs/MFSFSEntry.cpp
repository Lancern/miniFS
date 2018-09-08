#include "../../include/fs/MFSPartition.h"
#include "../../include/serialize/MFSDirectoryBlockSerializer.h"
#include "../../include/fs/MFSFSNodePool.h"



#define CEIL_DIV(a, b)      ((a) / (b) + ((a) % (b) ? 1 : 0))


MFSFSEntry::MFSFSEntry(MFSPartition::Internals partition, uint32_t fsnodeId)
    : _partition(partition), _meta(partition.GetEntryMeta(fsnodeId)), _fsnodeId(fsnodeId)
{
}

MFSPartition * MFSFSEntry::GetPartition() const
{
    return _partition.GetPartition();
}

uint32_t MFSFSEntry::GetFSNodeId() const
{
    return _fsnodeId;
}

MFSFSEntryType MFSFSEntry::GetEntryType() const
{
    return static_cast<MFSFSEntryType>(_meta->common.flags & MFS_FSENTRY_FLAG_FILE);
}

void MFSFSEntry::SetEntryType(MFSFSEntryType type)
{
    switch (type)
    {
    case MFSFSEntryType::Directory:
        _meta->common.flags &= ~MFS_FSENTRY_FLAG_FILE;
        break;
    case MFSFSEntryType::File:
        _meta->common.flags |= MFS_FSENTRY_FLAG_FILE;
        break;
    }
}

MFSDateTime MFSFSEntry::GetCreationTime() const
{
    return MFSDateTime(MFSGetPackedUnsignedValue(&_meta->common.creationTimestamp));
}

MFSDateTime MFSFSEntry::GetLastAccessTime() const
{
    return MFSDateTime(MFSGetPackedUnsignedValue(&_meta->common.lastAccessTimestamp));
}

MFSDateTime MFSFSEntry::GetLastModificationTime() const
{
    return MFSDateTime(MFSGetPackedUnsignedValue(&_meta->common.lastModTimestamp));
}

bool MFSFSEntry::GetHiddenFlag() const
{
    return static_cast<bool>(_meta->common.flags & MFS_FSENTRY_FLAG_HIDE);
}

bool MFSFSEntry::GetProtectedFlag() const
{
    return static_cast<bool>(_meta->common.flags & MFS_FSENTRY_FLAG_PROTECTED);
}

MFSFSEntryAccess MFSFSEntry::GetAccessAttributes() const
{
    DWORD read = _meta->common.flags & MFS_FSENTRY_ACCESS_READ;
    DWORD write = _meta->common.flags & MFS_FSENTRY_ACCESS_WRITE;
    DWORD execute = _meta->common.flags & MFS_FSENTRY_ACCESS_EXECUTE;
    return static_cast<MFSFSEntryAccess>(read | write | execute);
}

void MFSFSEntry::SetHiddenFlag(bool isHidden)
{
    if (isHidden)
        _meta->common.flags |= MFS_FSENTRY_FLAG_HIDE;
    else
        _meta->common.flags &= ~MFS_FSENTRY_FLAG_HIDE;
}

void MFSFSEntry::SetProtectedFlag(bool isProtected)
{
    if (isProtected)
        _meta->common.flags |= MFS_FSENTRY_FLAG_PROTECTED;
    else
        _meta->common.flags &= ~MFS_FSENTRY_FLAG_PROTECTED;
}

void MFSFSEntry::SetAccessAttributes(MFSFSEntryAccess access)
{
    _meta->common.flags &= ~14u;
    _meta->common.flags |= (static_cast<DWORD>(access) & 14u);
}

uint64_t MFSFSEntry::GetFileSize() const
{
    if (GetEntryType() == MFSFSEntryType::File)
        return MFSGetPackedUnsignedValue(&_meta->spec.fileMeta.size);
    else
        return 0;
}

bool MFSFSEntry::SetFileSize(uint64_t size)
{
    uint64_t timestamp = MFSGetCurrentTimestamp();
    MFSGetInteger64Struct(&_meta->common.lastAccessTimestamp, timestamp);
    MFSGetInteger64Struct(&_meta->common.lastModTimestamp, timestamp);

    uint64_t currentSize = GetFileSize();

    if (size == currentSize)
        return true;
    else if (size < currentSize)
        return TruncateFileSize(size);
    else    // size > currentSize
        return ExtendFileSize(size);
}

MFSBlockStream * MFSFSEntry::OpenDataStream()
{
    if (GetEntryType() == MFSFSEntryType::Directory)
        return new DataStream(this, _meta->common.firstBlockId);
    else
        return new DataStream(this, _meta->common.firstBlockId, 
            MFSGetPackedUnsignedValue(&_meta->spec.fileMeta.size));
}

auto MFSFSEntry::GetSubEntries() -> std::vector<std::pair<MFSString, std::unique_ptr<MFSFSEntry>>>
{
	std::vector<std::pair<MFSString, std::unique_ptr<MFSFSEntry>>> ret;
	auto callback = [&](const WalkDirectoryBlockParameters & params)
	{
		for (auto&& it : *params.blockObject)
			ret.emplace_back(it.first, new MFSFSEntry(_partition, it.second.fsnodeId));
	};
	WalkDirectoryBlocks(callback);
	return ret;
}

uint32_t MFSFSEntry::GetSubEntriesCount() const
{
    return _meta->spec.directoryMeta.childCount;
}

bool MFSFSEntry::ContainsSubEntry(const MFSString & name)
{
	bool exist = false;
	auto callback = [&](WalkDirectoryBlockParameters & params)
	{
		if (params.blockObject->FindDir(name))
		{
			exist = true;
            params.stopIteration = true;
		}
	};
	WalkDirectoryBlocks(callback);
	return exist;
}

MFSFSEntry * MFSFSEntry::GetSubEntry(const MFSString & name)
{
	MFSFSEntry* ret = nullptr;
	auto callback = [&](WalkDirectoryBlockParameters & params)
	{
		for (auto&& it : *params.blockObject)
			if (it.first == name)
			{
				ret = new MFSFSEntry(_partition, it.second.fsnodeId);
                params.stopIteration = true;
                break;
            }
	};
	WalkDirectoryBlocks(callback);
	return ret;
}

MFSFSEntry * MFSFSEntry::AddSubEntry(const MFSString & name)
{
    uint32_t fsnodeId = _partition.GetAvailableFSNodeId();
    if (fsnodeId == MFSFSNodePool::InvalidFSNodeId)
        return nullptr;

    return AddSubEntry(name, fsnodeId);
}

MFSFSEntry * MFSFSEntry::AddSubEntry(const MFSString & name, uint32_t fsnodeId)
{
    if (ContainsSubEntry(name))
        return nullptr;

    MFSFSEntry* ret = nullptr;
    auto callback = [&](WalkDirectoryBlockParameters & params)
    {
        auto item = params.blockObject->AddDir(name);
        if (item)
        {
            _partition.AllocateEntryMeta(fsnodeId);
            item->fsnodeId = fsnodeId;
            ++_meta->spec.directoryMeta.childCount;
            ret = new MFSFSEntry(_partition, fsnodeId);
            
            params.stopIteration = true;
            params.needWriteBack = true;
        }
    };
    WalkDirectoryBlocks(callback);
    if (ret == nullptr)
    {
        DWORD blockId = _partition.AllocateFrontBlock(_meta->common.firstBlockId);
        if (blockId == MFSBlockAllocationBitmap::InvalidBlockId)
            return nullptr;
        _meta->common.firstBlockId = blockId;

        WalkDirectoryBlocks(callback);
    }
    return ret;
}

bool MFSFSEntry::RemoveSubEntry(const MFSString & name)
{
	if (!ContainsSubEntry(name))
        return false;

	auto callback = [&](WalkDirectoryBlockParameters & params)
	{
		if (params.blockObject->EraseDir(name) != MFSFSNodePool::InvalidFSNodeId)
		{
            _partition.FreeEntryMeta(_fsnodeId);
            --_meta->spec.directoryMeta.childCount;
            if (params.blockObject->Empty())
            {
                _meta->common.firstBlockId = _partition.FreeChainedBlock(
                    _meta->common.firstBlockId,
                    static_cast<DWORD>(params.blockId));
            }
            else
            {
                params.needWriteBack = true;
            }
            params.stopIteration = true;
		}
    };
    WalkDirectoryBlocks(callback);

    return true;
}

bool MFSFSEntry::TruncateFileSize(uint64_t size)
{
    uint32_t blockSize = _partition.GetPartition()->GetDevice()->GetBlockSize();
    uint64_t truncatedBlocksCount = CEIL_DIV(size, blockSize);

    uint32_t blockPtr = _meta->common.firstBlockId;
    while (truncatedBlocksCount > 0 && blockPtr != MFSFileAllocationTable::InvalidBlockId)
    {
        --truncatedBlocksCount;
        blockPtr = _partition.GetNextChainedBlock(blockPtr);
    }

    if (truncatedBlocksCount > 0)
    {
        // Unexpected block pointer hit end of block chain.
        MFSGetInteger64Struct(&_meta->spec.fileMeta.size, size);
        return true;
    }
    else
    {
        while (_partition.GetNextChainedBlock(blockPtr) != MFSFileAllocationTable::InvalidBlockId)
        {
            _partition.FreeBlockAfter(blockPtr);
        }
        MFSGetInteger64Struct(&_meta->spec.fileMeta.size, size);
        return true;
    }
}

bool MFSFSEntry::ExtendFileSize(uint64_t size)
{
    uint32_t blockSize = _partition.GetPartition()->GetDevice()->GetBlockSize();
    uint32_t extendedBlocksCount = static_cast<uint32_t>(CEIL_DIV(size, blockSize));

    uint32_t blockPtr = _meta->common.firstBlockId;
    uint32_t currentBlocksCount = 0;
    if (blockPtr != MFSFileAllocationTable::InvalidBlockId)
    {
        currentBlocksCount = 1;
        while (_partition.GetNextChainedBlock(blockPtr) != MFSFileAllocationTable::InvalidBlockId)
        {
            blockPtr = _partition.GetNextChainedBlock(blockPtr);
            ++currentBlocksCount;
        }
    }

    uint32_t required = extendedBlocksCount > currentBlocksCount
        ? extendedBlocksCount - currentBlocksCount
        : 0;
    
    if (required == 0)
    {
        MFSGetInteger64Struct(&_meta->spec.fileMeta.size, size);
        return true;
    }
    else
    {
        // Try allocate $required chained blocks.
        uint32_t allocatedFirst = _partition.AllocateBlockChain(required);
        if (allocatedFirst == MFSBlockAllocationBitmap::InvalidBlockId)
            return false;

        if (currentBlocksCount == 0)
            _meta->common.firstBlockId = allocatedFirst;
        else
            _partition.AppendTailBlock(blockPtr, allocatedFirst);

        MFSGetInteger64Struct(&_meta->spec.fileMeta.size, size);
        return true;
    }
}
