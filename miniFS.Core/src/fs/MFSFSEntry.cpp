#include "../../include/fs/MFSFSEntry.h"
#include "../../include/serialize/MFSDirectoryBlockSerializer.h"
#include "../../include/fs/MFSFSNodePool.h"

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
	if (ContainsSubEntry(name)) 
        return nullptr;
	MFSFSEntry* ret = nullptr;
	auto callback = [&](WalkDirectoryBlockParameters & params)
	{
		auto item = params.blockObject->AddDir(name);
        // TODO: FIX ME: MFSFSEntry::AddSubEntry(const MFSString &)
        // TODO: Allocate a new fsnode and assign this node to directory block.
		if (item)
		{
			ret = new MFSFSEntry(_partition, item->fsnodeId);
            params.stopIteration = true;
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

MFSFSEntry * MFSFSEntry::RemoveSubEntry(const MFSString & name)
{
	if (!ContainsSubEntry(name)) 
        return nullptr;
	MFSFSEntry* ret = nullptr;
	auto callback = [&](WalkDirectoryBlockParameters & params)
	{
		uint32_t fsnodeId = params.blockObject->EraseDir(name);
		if (fsnodeId != MFSFSNodePool::InvalidFSNodeId)
		{
			ret = new MFSFSEntry(_partition, fsnodeId);
			if (params.blockObject->Empty())
                _partition.FreeChainedBlock(_meta->common.firstBlockId, params.blockId);
            params.stopIteration = true;
		}
    };

    return ret;
}
