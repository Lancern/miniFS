#pragma once

#include <cstdint>

#define MFS_MAGIC_SEQ       0x0053462D696E696D
#define MFS_VER             1

struct MFSFSMasterInfo
{
    uint64_t magicSeq;      // mini-FS文件系统魔数。
    uint32_t mfsVer;        // mini-FS文件系统版本号
    uint32_t reversed;
    uint32_t totalBlocks;   // 总的数据块数量
    uint32_t freeBlocks;    // 处于空闲状态的数据块数量。
};

#define MFS_FSENTRY_FLAG_DIRECTORY  0
#define MFS_FSENTRY_FLAG_FILE       1
#define MFS_FSENTRY_ACCESS_READ     2
#define MFS_FSENTRY_ACCESS_WRITE    4
#define MFS_FSENTRY_ACCESS_EXECUTE  8
#define MFS_FSENTRY_FLAG_HIDE       16
#define MFS_FSENTRY_FLAG_PROTECTED  32

struct MFSFSEntryCommonMeta
{
    uint32_t flags;
    uint32_t firstBlockId;          // 第一个数据块编号
    uint64_t creationTimestamp;     // 创建时间戳
    uint64_t lastAccessTimestamp;   // 上次访问时间戳
    uint64_t lastModTimestamp;      // 上次修改时间戳
    uint32_t refCount;              // 引用计数
};

struct MFSFSFileEntryMeta
{
    uint64_t size;      // 文件字节大小
};

struct MFSFSDirectoryEntryMeta
{
    uint32_t childCount;        // 目录下直接子文件系统项数目
    uint32_t reserved;
};

struct MFSFSEntryMeta
{
    MFSFSEntryCommonMeta common;
    uint32_t reserved;
    union
    {
        MFSFSFileEntryMeta fileMeta;
        MFSFSDirectoryEntryMeta directoryMeta;
    } spec;
};

struct MFSFSDirectoryBlockMasterInfo
{
    uint32_t itemsCount;        // 目录数据块中的目录项数量。
};

struct MFSFSDirectoryItem
{
    uint32_t nameOffset;        // 目录项的名称在目录项名称堆区域内的起始偏移量。
    uint32_t fsnodeId;          // 表示与该目录项对应的文件系统节点。
};
