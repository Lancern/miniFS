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
        MFSPartition * _partition;

        MFSPartition * GetPartition() const;

        DWORD AllocateDeviceBlock();
        bool AllocateDeviceBlock(DWORD blockId);
        bool FreeDeviceBlock(DWORD blockId);

        DWORD AllocateTailBlock(DWORD firstBlockId);
        DWORD AllocateFrontBlock(DWORD firstBlockId);

		DWORD FreeChainedBlock(DWORD firstBlockId, DWORD blockId);

        DWORD AllocateEntryMeta();
        bool AllocateEntryMeta(DWORD fsnodeId);
        MFSFSEntryMeta * GetEntryMeta(uint32_t fsnodeId) const;
        bool FreeEntryMeta(uint32_t fsnodeId);

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

    MFSStream * OpenDataStream()
        打开文件系统项目数据流对象。

    std::vector<std::unique_ptr<MFSFSEntry>> GetSubEntries()
        当文件系统节点所表示的文件系统项为一个目录时，获取该目录下所有子文件系统项对象。

    MFSFSEntry * GetSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，获取该目录下指定名称的子文件系统项对象。若目录下没有指定的名称，返回 nullptr。

    MFSFSEntry * AddSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，尝试在目录下创建指定名称的子文件系统项对象。若创建失败或指定的名称已经存在，返回 nullptr。

    MFSFSEntry * RemoveSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，尝试在目录下删除指定名称的子文件系统项对象。若删除成功，返回删除的项的文件系统项实例对象；
        否则返回 nullptr。

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

    MFSBlockStream * OpenDataStream();

    auto GetSubEntries()->std::vector<std::pair<MFSString, std::unique_ptr<MFSFSEntry>>>;
    bool ContainsSubEntry(const MFSString & name);
    MFSFSEntry * GetSubEntry(const MFSString & name);
    MFSFSEntry * AddSubEntry(const MFSString & name);
    MFSFSEntry * RemoveSubEntry(const MFSString & name);

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

