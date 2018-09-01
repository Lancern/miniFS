#pragma once

#include <cstdint>

#define MFS_MAGIC_SEQ       0x0053462D696E696D
#define MFS_VER             1

struct MFSFSMasterInfo
{
    uint64_t magicSeq;      // mini-FS�ļ�ϵͳħ����
    uint32_t mfsVer;        // mini-FS�ļ�ϵͳ�汾��
    uint32_t reversed;
    uint32_t totalBlocks;   // �ܵ����ݿ�����
    uint32_t freeBlocks;    // ���ڿ���״̬�����ݿ�������
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
    uint32_t firstBlockId;          // ��һ�����ݿ���
    uint64_t creationTimestamp;     // ����ʱ���
    uint64_t lastAccessTimestamp;   // �ϴη���ʱ���
    uint64_t lastModTimestamp;      // �ϴ��޸�ʱ���
    uint32_t refCount;              // ���ü���
};

struct MFSFSFileEntryMeta
{
    uint64_t size;      // �ļ��ֽڴ�С
};

struct MFSFSDirectoryEntryMeta
{
    uint32_t childCount;        // Ŀ¼��ֱ�����ļ�ϵͳ����Ŀ
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
    uint32_t itemsCount;        // Ŀ¼���ݿ��е�Ŀ¼��������
};

struct MFSFSDirectoryItem
{
    uint32_t nameOffset;        // Ŀ¼���������Ŀ¼�����ƶ������ڵ���ʼƫ������
    uint32_t fsnodeId;          // ��ʾ���Ŀ¼���Ӧ���ļ�ϵͳ�ڵ㡣
};
