#include "../../include/fs/MFSPartition.h"


MFSFSEntry::DataStream::DataStream(MFSFSEntry * entry, uint32_t firstBlockId)
    : ChainedBlockStream(entry->GetPartition(), firstBlockId), _entry(entry)
{
}

MFSFSEntry::DataStream::DataStream(MFSFSEntry * entry, uint32_t firstBlockId, uint64_t length)
    : ChainedBlockStream(entry->GetPartition(), firstBlockId, length), _entry(entry)
{
}

uint32_t MFSFSEntry::DataStream::Read(void * lpBuffer, uint32_t dwBufferSize, uint32_t dwNumberOfBytesToRead)
{
    MFSGetInteger64Struct(&_entry->_meta->common.lastAccessTimestamp, MFSGetCurrentTimestamp());
    return ChainedBlockStream::Read(lpBuffer, dwBufferSize, dwNumberOfBytesToRead);
}

uint32_t MFSFSEntry::DataStream::Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite)
{
    uint64_t timestamp = MFSGetCurrentTimestamp();
    MFSGetInteger64Struct(&_entry->_meta->common.lastAccessTimestamp, timestamp);
    MFSGetInteger64Struct(&_entry->_meta->common.lastModTimestamp, timestamp);
    return ChainedBlockStream::Write(lpBuffer, dwNumberOfBytesToWrite);
}
