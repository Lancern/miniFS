#include "../../include/fs/MFSFSEntry.h"

MFSFSEntry::MFSFSEntry(MFSPartition * partition, uint32_t fsnodeId)
    : _partition(partition), _meta(partition->GetEntryMeta(fsnodeId)), _fsnodeId(fsnodeId)
{
}

MFSPartition * MFSFSEntry::GetPartition() const
{
    return _partition;
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
