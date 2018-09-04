#pragma once

#include "../MFSString.h"
#include "../MFSDateTime.h"
#include "../device/MFSBlockDevice.h"
#include "../stream/MFSBlockStream.h"
#include "MFSMetas.h"
#include "MFSBlockAllocationBitmap.h"
#include "MFSFileAllocationTable.h"
#include "MFSFSNodePool.h"
#include "MFSDirectoryBlock.h"
#include <memory>
#include <utility>
#include <vector>


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

class MFSPartition::Internals
    对外部友元代码提供 MFSPartition 内部访问接口。

    构造器：

    MFSPartition::Internals::Internals(MFSPartition * host)
        从指定的 MFSPartition 对象创建其内部接口的包装。

    成员函数：

    MFSPartition * MFSPartition::Internals::GetPartition() const
        获取内部的 MFSPartition 对象。

    DWORD MFSPartition::Internals::AllocateDeviceBlock()
        在内部的 MFSPartition 对象上分配一个设备块并返回分配的设备块编号并建立其 FAT 块链。
        若分配失败，返回 MFSBlockAllocationBitmap::InvalidBlockId。

    bool MFSPartition::Internals::AllocateDeviceBlock(DWORD blockId)
        在内部的 MFSPartition 对象上尝试分配指定编号的设备块并建立其 FAT 块链。
        @return 一个 bool 值指示分配操作是否成功。

    bool MFSPartition::Internals::FreeDeviceBlock(DWORD blockId)
        在内部的 MFSPartition 对象上释放给定编号的设备块。
        @return 一个 bool 值指示释放操作是否成功。

    DWORD MFSPartition::Internals::AllocateTailBlock(DWORD firstBlockId)
        在内部的 MFSPartition 对象上分配一个设备块并将其添加至一个块链的尾部。
        @param firstBlockId 要在尾部分配设备块的块链的第一个块编号。
        @return 新分配的块编号。若分配失败，返回 MFSBlockAllocationBitmap::InvalidBlockId。

    DWORD MFSPartition::Internals::AllocateFrontBlock(DWORD firstBlockId)
        在内部的 MFSPartition 对象上分配一个设备块并将其添加至一个块链的头部。
        @param firstBlockId 要在头部分配设备块的块链的第一个块编号。
        @retrun 新分配的块编号。若分配失败，返回 MFSBlockAllocationBitmap::InvalidBlockId。

    DWORD MFSPartition::Internals::AllocateBlockChain(DWORD numberOfBlocks)
        在内部的 MFSPartition 对象上分配一个包含指定块个数的块链。
        @param numberOfBlocks 要分配的块链中块的个数。
        @return 分配出的块链的第一块编号。
        若无法分配指定个数个设备块，返回 MFSBlockAllocationBitmap::InvalidBlockId。

    DWORD MFSPartition::Internals::FreeChainedBlock(DWORD firstBlockId, DWORD blockId)
        在内部的 MFSPartition 对象上释放块链上的一个数据块。
        @param firstBlockId 块链的第一个设备块编号。
        @param blockId 要释放的块的编号。
        @return 释放操作完成后块链的首块编号。

    DWORD MFSPartition::Internals::FreeBlockAfter(DWORD position)
        在内部的 MFSPartition 对象上释放给定块在块链上的下一个设备块。
        @param position 块 position 在块链上的下一个设备块将被释放。
        @return 被释放的块编号。

    DWORD MFSPartition::Internals::AppendTailBlock(DWORD firstBlockId, DWORD blockId)
        将给定的块添加至给定的块链末尾。
        @param firstBlockId 块链的第一块编号。
        @param blockId 要添加到块链末尾的块编号。
        @return 操作结束后块链的第一块编号。

    DWORD GetNextChainedBlock(DWORD blockId) const
        获取内部 MFSPartition 对象上指定设备块在块链上的下一块编号。

    DWORD MFSPartition::Internals::GetAvailableFSNodeId()
        在内部的 MFSPartition 对象上获取下一个可用的文件系统节点的编号。
        若没有可用的文件系统节点，返回 MFSFSNodePool::InvalidFSNodeId。

    bool MFSPartition::Internals::AllocateEntryMeta(DWORD fsnodeId)
        在内部的 MFSPartition 对象上递增一个文件系统节点元数据结构的引用计数。
        返回的节点已经被保证初始化。
        @param fsnodeId 要分配的节点在节点池中的编号。
        @return 一个 bool 值指示目标节点在操作之前的引用计数是否为 0。

    MFSFSEntryMeta * GetEntryMeta(uint32_t fsnodeId) const
        获取编号为给定值的节点元数据。

    bool FreeEntryMeta(uint32_t fsnodeId)
        递减给定的文件系统节点的引用计数并在引用计数降至零时释放其链接到的块链。

    MFSBlockStream * MFSPartition::Internals::OpenBlockStream(DWORD firstBlock)
        在内部的 MFSPartition 对象上打开一个按块链组织的流对象。流对象的长度将会被对齐到块边界。
        @param firstBlock 打开的流对象的基础块链的第一块编号。

    MFSBlockStream * MFSPartition::Internals::OpenBlockStream(DWORD firstBlock, UINT64 length)
        在内部的 MFSPartition 对象上打开一个按块链组织的流对象。
        @param firstBlock 打开的流对象的基础块链的第一块编号。
        @param length 打开的流对象的长度。

*/

class MFSFSEntry;

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
        using ChainedBlockStream = MFSPartition::ChainedBlockStream;

        MFSPartition * _partition;

        MFSPartition * GetPartition() const;

        DWORD AllocateDeviceBlock();
        bool AllocateDeviceBlock(DWORD blockId);
        bool FreeDeviceBlock(DWORD blockId);

        DWORD AllocateTailBlock(DWORD firstBlockId);
        DWORD AllocateFrontBlock(DWORD firstBlockId);
        DWORD AllocateBlockChain(DWORD numberOfBlocks);
		DWORD FreeChainedBlock(DWORD firstBlockId, DWORD blockId);
        DWORD FreeBlockAfter(DWORD position);

        DWORD AppendTailBlock(DWORD firstBlockId, DWORD blockId);

        DWORD GetNextChainedBlock(DWORD blockId) const;

        DWORD GetAvailableFSNodeId();
        bool AllocateEntryMeta(DWORD fsnodeId);
        MFSFSEntryMeta * GetEntryMeta(uint32_t fsnodeId) const;
        bool FreeEntryMeta(uint32_t fsnodeId);

        MFSBlockStream * OpenBlockStream(DWORD firstBlock);
        MFSBlockStream * OpenBlockStream(DWORD firstBlock, UINT64 length);

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

    MFSFSEntry * GetRoot();

    Internals GetInternalObject();

    void Flush();
    void Close();

private:
    std::unique_ptr<MFSBlockDevice> _device;
    MFSFSMasterInfo _master;
    std::unique_ptr<MFSBlockAllocationBitmap> _blockAllocation;
    std::unique_ptr<MFSFileAllocationTable> _blockChain;
    std::unique_ptr<MFSFSNodePool> _fsnodePool;
    bool _validDevice;
    
    void LoadDevice(MFSBlockDevice * device);

    bool LoadMasterInfo(MFSBlockStream * deviceStream);
    bool LoadBlockAllocationManager(MFSBlockStream * deviceStream);
    bool LoadAllocationTable(MFSBlockStream * deviceStream);
    bool LoadFSNodePool(MFSBlockStream * deviceStream);
};



/*

class MFSFSEntry
表示文件系统树的一个节点。

    构造器：

    MFSFSEntry::MFSFSEntry(MFSPartition * partition, uint32_t fsnodeId)
        从给定的文件系统分区以及文件系统编号初始化 MFSFSEntry 类的新实例。

    成员函数：

    MFSPartition * MFSFSEntry::GetPartition() const
        获取当前 MFSFSEntry 所属分区的对象实例。

    uint32_t MFSFSEntry::GetFSNodeId() const
        获取当前 MFSFSEntry 对应的文件系统节点编号。

    MFSFSEntryType MFSFSEntry::GetEntryType() const
        获取当前 MFSFSEntry 所表示的文件系统项目的类型。

    void MFSFSEntry::SetEntryType(MFSFSEntryType type)
        设置当前 MFSFSEntry 所表示的文件系统项目的类型。

    MFSDateTime MFSFSEntry::GetCreationTime() const
        获取当前文件系统节点的创建时间。

    MFSDateTime MFSFSEntry::GetLastAccessTime() const
        获取当前文件系统节点的上一次访问时间。

    MFSDateTime MFSFSEntry::GetLastModificationTime() const
        获取当前文件系统节点上一次被修改的时间。

    bool MFSFSEntry::GetHiddenFlag() const
        获取文件系统节点的隐藏位。

    bool MFSFSEntry::GetProtectedFlag() const
        获取文件系统节点的保护位。

    MFSFSEntryAccess MFSFSEntry::GetAccessAttribute() const
        获取文件系统节点的访问控制属性。

    void MFSFSEntry::SetHiddenFlag(bool isHidden)
        设置文件系统节点的隐藏位。

    void MFSFSEntry::SetProtectedFlag(bool isProtected)
        设置文件系统节点的保护位。

    void MFSFSEntry::SetAccessAttributes(MFSFSEntryAccess access)
        设置文件系统节点的访问控制属性。

    uint64_t MFSFSEntry::GetFileSize() const
        当文件系统节点所表示的文件系统项为一个文件时，返回该文件的字节大小；否则返回 0。

    bool MFSFSEntry::SetFileSize(uint64_t size)
        当文件系统节点所表示的文件系统项为一个文件时，设置该文件的字节大小。若设置的值大于文件的现有大小，
        所需的新空间会被分配给该文件；若设置的值小于文件的现有大小，文件会被截断。
        @return 一个 bool 值指示操作是否成功。

    MFSStream * MFSFSEntry::OpenDataStream()
        打开文件系统项目数据流对象。

    std::vector<std::unique_ptr<MFSFSEntry>> MFSFSEntry::GetSubEntries()
        当文件系统节点所表示的文件系统项为一个目录时，获取该目录下所有子文件系统项对象。

    MFSFSEntry * MFSFSEntry::GetSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，获取该目录下指定名称的子文件系统项对象。若目录下没有指定的名称，返回 nullptr。

    MFSFSEntry * MFSFSEntry::AddSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，尝试在目录下创建指定名称的子文件系统项对象。
        若创建失败或指定的名称已经存在，返回 nullptr。

    MFSFSEntry * MFSFSEntry::AddSubEntry(const MFSString & name, uint32_t fsnodeId)
        当文件系统节点所表示的文件系统项为一个目录时，尝试在目录下创建给定名称的目录项并将其硬链接到
        给定的文件系统节点上。

    MFSFSEntry * MFSFSEntry::RemoveSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，尝试在目录下删除指定名称的子文件系统项对象。
        @return 返回一个 bool 值指示删除是否成功。

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
    MFSFSEntry(MFSPartition::Internals partition, uint32_t fsnodeId);

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
    bool SetFileSize(uint64_t size);

    MFSBlockStream * OpenDataStream();

    auto GetSubEntries()->std::vector<std::pair<MFSString, std::unique_ptr<MFSFSEntry>>>;
    bool ContainsSubEntry(const MFSString & name);
    MFSFSEntry * GetSubEntry(const MFSString & name);
    MFSFSEntry * AddSubEntry(const MFSString & name);
    MFSFSEntry * AddSubEntry(const MFSString & name, uint32_t fsnodeId);
    bool RemoveSubEntry(const MFSString & name);

private:
    struct WalkDirectoryBlockParameters
    {
        uint64_t blockId;
        MFSBlockStream * blockStream;
        MFSDirectoryBlock * blockObject;
        bool stopIteration;
    };

    MFSPartition::Internals _partition;
    MFSFSEntryMeta * _meta;
    uint32_t _fsnodeId;

    template <typename Callback>
    void WalkDirectoryBlocks(Callback callback);

    bool TruncateFileSize(uint64_t size);
    bool ExtendFileSize(uint64_t size);
};

template<typename Callback>
inline void MFSFSEntry::WalkDirectoryBlocks(Callback callback)
{
    std::unique_ptr<MFSBlockStream> stream(OpenDataStream());
    MFSDirectoryBlockSerializer serializer(_partition.GetPartition()->GetDevice()->GetBlockSize());
    while (stream->HasNext())
    {
        std::unique_ptr<MFSDirectoryBlock> block(serializer.Deserialize(stream.get()));

        WalkDirectoryBlockParameters params;
        params.blockId = stream->GetCurrentBlockId();
        params.blockStream = stream.get();
        params.blockObject = block.get();
        params.stopIteration = false;
        callback(params);

        if (params.stopIteration)
            break;
    }
    stream->Close();
}

