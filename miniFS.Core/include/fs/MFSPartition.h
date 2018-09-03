#pragma once

#include "../device/MFSBlockDevice.h"
#include "../stream/MFSBlockStream.h"
#include "MFSFSEntry.h"
#include "MFSMetas.h"
#include "MFSBlockAllocationBitmap.h"
#include "MFSFileAllocationTable.h"
#include <memory>


/*

class MFSPartition
表示一个文件系统分区。

    构造器：

    MFSPartition::MFSPartition(MFSBlockDevice * device)
        从给定的块设备初始化 MFSPartition 类的新实例。

    成员函数：

    MFSBlockDevice * MFSPartition::GetDevice() const
        获取文件系统分区的基础块设备。

    bool MFSPartition::IsRaw() const
        获取一个 bool 值，该值指示当前的文件系统分区是否未格式化。

    UINT64 GetTotalSpaceInBytes() const
        获取当前分区的总空间大小。

    UINT64 GetFreeSpaceInBytes() const
        获取当前分区的空闲空间大小。

    void MFSPartition::BuildFileSystem()
        在基础块设备上建立基础文件系统结构（格式化）。

    MFSFSEntry * MFSPartition::GetRoot() const
        获取该分区上的文件系统树根节点。

    void MFSPartition::Flush()
        将所有修改写入基础设备。

    void MFSPartition::Close()
        将所有修改写入基础设备并关闭当前的分区。

*/

class MFSPartition
{
private:
    class ChainedBlockStream
        : public MFSBlockStream
    {
    public:
        ChainedBlockStream(MFSPartition * partition, DWORD firstBlockId);
        ChainedBlockStream(MFSPartition * partition, DWORD firstBlockId, UINT64 length);

        MFSPartition * GetPartition() const;

        bool HasNext() const override;

        UINT64 GetLength() const override;
        UINT64 GetPosition() const override;

        bool Seek(MFSStreamSeekOrigin origin, INT64 offset) override;

    protected:
        UINT64 OnBlockSwap(UINT64 currentBlock) override;

    private:
        MFSPartition * _partition;
        DWORD _firstBlock;
        DWORD _currentBlock;
        DWORD _blockOffset;
        UINT64 _length;

        DWORD GetNextBlockId(DWORD current) const;
        DWORD GetNextBlockId() const;

        bool SeekBegin(INT64 offset);
        bool SeekRelative(INT64 offset);
        bool SeekEnd(INT64 offset);
    };

public:
    class Internals
    {
    public:
        Internals(MFSPartition * host);

    private:
        MFSPartition * _partition;

        MFSPartition * GetPartition() const;

        DWORD AllocateDeviceBlock();
        bool AllocateDeviceBlock(DWORD blockId);
        bool FreeDeviceBlock(DWORD blockId);

        DWORD AllocateTailBlock(DWORD firstBlockId);
        DWORD AllocateFrontBlock(DWORD firstBlockId);

		DWORD FreeChainedBlock(DWORD firstBlockId, DWORD blockId);

        MFSFSEntryMeta * GetEntryMeta(uint32_t fsnodeId) const;

        MFSStream * OpenBlockStream(DWORD firstBlock);
        MFSStream * OpenBlockStream(DWORD firstBlock, UINT64 length);

        friend class MFSFSEntry;
    };

    MFSPartition(MFSBlockDevice * device);
    ~MFSPartition();

    MFSBlockDevice * GetDevice() const;
    bool IsValidDevice() const;

    bool IsRaw() const;
    UINT64 GetTotalSpaceInBytes() const;
    UINT64 GetFreeSpaceInBytes() const;
    
    void BuildFileSystem();

    MFSFSEntry * GetRoot() const;

    Internals GetInternalObject();

    void Flush();
    void Close();

private:
    std::unique_ptr<MFSBlockDevice> _device;
    MFSFSMasterInfo _master;
    std::unique_ptr<MFSBlockAllocationBitmap> _blockAllocation;
    std::unique_ptr<MFSFileAllocationTable> _blockChain;
    std::unique_ptr<MFSFSEntryMeta[]> _fsnodePool;
    bool _validDevice;
    
    void LoadDevice(MFSBlockDevice * device);

    bool LoadMasterInfo(MFSBlockStream * deviceStream);
    bool LoadBlockAllocationManager(MFSBlockStream * deviceStream);
    bool LoadAllocationTable(MFSBlockStream * deviceStream);
    bool LoadFSNodePool(MFSBlockStream * deviceStream);
};
