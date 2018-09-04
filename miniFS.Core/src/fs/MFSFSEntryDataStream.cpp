#include "../../include/fs/MFSPartition.h"


MFSFSEntry::DataStream::DataStream(MFSFSEntry * entry, DWORD firstBlockId)
    : ChainedBlockStream(entry->GetPartition(), firstBlockId), _entry(entry)
{
}

MFSFSEntry::DataStream::DataStream(MFSFSEntry * entry, DWORD firstBlockId, UINT64 length)
    : ChainedBlockStream(entry->GetPartition(), firstBlockId, length), _entry(entry)
{
}

DWORD MFSFSEntry::DataStream::Read(LPVOID lpBuffer, DWORD dwBufferSize, DWORD dwNumberOfBytesToRead)
{
    MFSGetInteger64Struct(&_entry->_meta->common.lastAccessTimestamp, MFSGetCurrentTimestamp());
    return ChainedBlockStream::Read(lpBuffer, dwBufferSize, dwNumberOfBytesToRead);
}

DWORD MFSFSEntry::DataStream::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite)
{
    uint64_t timestamp = MFSGetCurrentTimestamp();
    MFSGetInteger64Struct(&_entry->_meta->common.lastAccessTimestamp, timestamp);
    MFSGetInteger64Struct(&_entry->_meta->common.lastModTimestamp, timestamp);
    return ChainedBlockStream::Write(lpBuffer, dwNumberOfBytesToWrite);
}
