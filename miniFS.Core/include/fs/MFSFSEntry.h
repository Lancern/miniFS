#pragma once

#include "MFSPartition.h"
#include "MFSMetas.h"
#include "../stream/MFSStream.h"
#include "../MFSString.h"
#include "../MFSDateTime.h"
#include <vector>
#include <memory>


/*

class MFSFSEntry
表示文件系统树的一个节点。

*/

enum MFSFSEntryType
{
    Directory = 0,
    File = 1
};

enum MFSFSEntryAccess
{
    Read = MFS_FSENTRY_ACCESS_READ,
    Write = MFS_FSENTRY_ACCESS_WRITE,
    Execute = MFS_FSENTRY_ACCESS_EXECUTE
};

class MFSFSEntry
{
public:
    MFSFSEntry(MFSPartition * partition, uint32_t fsnodeId);

    MFSPartition * GetPartition() const;
    uint32_t GetFSNodeId() const;

    MFSFSEntryType GetEntryType() const;
    void SetEntryType(MFSFSEntryType type);

    MFSDateTime GetCreationTime() const;
    MFSDateTime GetLastAccessTime() const;
    MFSDateTime GetLastModificationTime() const;

    bool GetHiddenFlag() const;
    bool GetProtectedFlag() const;
    MFSFSEntryAccess GetAccessAttributes() const;

    void SetHiddenFlag(bool isHidden);
    void SetProtectedFlag(bool isProtected);
    void SetAccessAttributes(MFSFSEntryAccess access);

    uint64_t GetFileSize() const;

    MFSStream * OpenDataStream();

    std::vector<std::unique_ptr<MFSFSEntry>> GetSubEntries();
    MFSFSEntry * GetSubEntry(const MFSString & name);
    MFSFSEntry * AddSubEntry(const MFSString & name);
    bool RemoveSubEntry(const MFSString & name);

private:
    MFSPartition * _partition;
    MFSFSEntryMeta * _meta;
    uint32_t _fsnodeId;
};
