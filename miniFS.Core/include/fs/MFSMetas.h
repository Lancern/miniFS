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

#define MFS_FSENTRY_FLAG_FILE           0x00000001
#define MFS_FSENTRY_ACCESS_READ         0x00000002
#define MFS_FSENTRY_ACCESS_WRITE        0x00000004
#define MFS_FSENTRY_ACCESS_EXECUTE      0x00000008
#define MFS_FSENTRY_FLAG_HIDE           0x00000010
#define MFS_FSENTRY_FLAG_PROTECTED      0x00000020

// 32位对齐友好的小端编码64位整数值。
struct MFS_INTEGER64
{
    uint32_t low;
    uint32_t high;
};

inline int64_t MFSGetPackedSignedValue(const MFS_INTEGER64 * i64)
{
    int64_t result = static_cast<int64_t>(MFSGetPackedUnsignedValue(i64));
    return result;
}

inline uint64_t MFSGetPackedUnsignedValue(const MFS_INTEGER64 * i64)
{
    uint64_t result = (i64->high << 32) | i64->low;
    return result;
}

inline void MFSGetInteger64Struct(MFS_INTEGER64 * i64, uint64_t value)
{
    i64->high = value >> 32;
    i64->low = value & 0xFFFFFFFF;
}

inline void MFSGetInteger64Struct(MFS_INTEGER64 * i64, int64_t value)
{
    MFSGetInteger64Struct(i64, static_cast<uint64_t>(value));
}

struct MFSFSEntryCommonMeta
{
    uint32_t        flags;
    uint32_t        firstBlockId;          // 第一个数据块编号
    MFS_INTEGER64   creationTimestamp;     // 创建时间戳
    MFS_INTEGER64   lastAccessTimestamp;   // 上次访问时间戳
    MFS_INTEGER64   lastModTimestamp;      // 上次修改时间戳
    uint32_t        refCount;              // 引用计数
};

static_assert(sizeof(MFSFSEntryCommonMeta) == 36, "Unexpected sizeof strcut MFSFSEntryCommonMeta.");
static_assert(alignof(MFSFSEntryCommonMeta) == 4, "Unexpected alignment of struct MFSFSEntryCommonMeta.");

struct MFSFSFileEntryMeta
{
    MFS_INTEGER64 size;         // 文件大小
};

static_assert(sizeof(MFSFSFileEntryMeta) == 8, "Unexpected size of struct MFSFSFileEntryMeta.");
static_assert(alignof(MFSFSFileEntryMeta) == 4, "Unexpected alignment of struct MFSFSFileEntryMeta.");

struct MFSFSDirectoryEntryMeta
{
    uint32_t childCount;        // 目录下直接子文件系统项数目
    uint32_t reserved;
};

static_assert(sizeof(MFSFSDirectoryEntryMeta) == 8, "Unexpected size of struct MFSFSDirectoryEntryMeta.");
static_assert(alignof(MFSFSDirectoryEntryMeta) == 4, "Unexpected alignment of struct MFSFSDirectoryEntryMeta.");

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

static_assert(sizeof(MFSFSEntryMeta) == 48, "Unexpected size of struct MFSFSEntryMeta.");
static_assert(alignof(MFSFSEntryMeta) == 4, "Unexpected alignment of struct MFSFSEntryMeta.");

struct MFSFSDirectoryBlockMasterInfo
{
    uint32_t itemsCount;        // 目录数据块中的目录项数量。
};

static_assert(sizeof(MFSFSDirectoryBlockMasterInfo) == 4, "Unexpected size of struct MFSFSDirectoryBlockMasterInfo.");
static_assert(alignof(MFSFSDirectoryBlockMasterInfo) == 4, "Unexpected alignment of struct MFSFSDirectoryBlockMasterInfo.");

struct MFSFSDirectoryItem
{
    uint32_t fsnodeId;          // 表示与该目录项对应的文件系统节点。
};

static_assert(sizeof(MFSFSDirectoryItem) == 4, "Unexpected size of struct MFSFSDirectoryItem.");
static_assert(alignof(MFSFSDirectoryItem) == 4, "Unexpected alignment of struct MFSFSDirectoryItem.");
