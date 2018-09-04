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
��ʾһ���ļ�ϵͳ������

    ��������

    MFSPartition::MFSPartition(MFSBlockDevice * device)
        �Ӹ����Ŀ��豸��ʼ�� MFSPartition �����ʵ����

    ��Ա������

    MFSBlockDevice * MFSPartition::GetDevice() const
        ��ȡ�ļ�ϵͳ�����Ļ������豸��

    bool MFSPartition::IsRaw() const
        ��ȡһ�� bool ֵ����ֵָʾ��ǰ���ļ�ϵͳ�����Ƿ�δ��ʽ����

    UINT64 GetTotalSpaceInBytes() const
        ��ȡ��ǰ�������ܿռ��С��

    UINT64 GetFreeSpaceInBytes() const
        ��ȡ��ǰ�����Ŀ��пռ��С��

    void MFSPartition::BuildFileSystem()
        �ڻ������豸�Ͻ��������ļ�ϵͳ�ṹ����ʽ������

    MFSFSEntry * MFSPartition::GetRoot() const
        ��ȡ�÷����ϵ��ļ�ϵͳ�����ڵ㡣

    void MFSPartition::Flush()
        �������޸�д������豸��

    void MFSPartition::Close()
        �������޸�д������豸���رյ�ǰ�ķ�����

class MFSPartition::Internals
    ���ⲿ��Ԫ�����ṩ MFSPartition �ڲ����ʽӿڡ�

    ��������

    MFSPartition::Internals::Internals(MFSPartition * host)
        ��ָ���� MFSPartition ���󴴽����ڲ��ӿڵİ�װ��

    ��Ա������

    MFSPartition * MFSPartition::Internals::GetPartition() const
        ��ȡ�ڲ��� MFSPartition ����

    DWORD MFSPartition::Internals::AllocateDeviceBlock()
        ���ڲ��� MFSPartition �����Ϸ���һ���豸�鲢���ط�����豸���Ų������� FAT ������
        ������ʧ�ܣ����� MFSBlockAllocationBitmap::InvalidBlockId��

    bool MFSPartition::Internals::AllocateDeviceBlock(DWORD blockId)
        ���ڲ��� MFSPartition �����ϳ��Է���ָ����ŵ��豸�鲢������ FAT ������
        @return һ�� bool ֵָʾ��������Ƿ�ɹ���

    bool MFSPartition::Internals::FreeDeviceBlock(DWORD blockId)
        ���ڲ��� MFSPartition �������ͷŸ�����ŵ��豸�顣
        @return һ�� bool ֵָʾ�ͷŲ����Ƿ�ɹ���

    DWORD MFSPartition::Internals::AllocateTailBlock(DWORD firstBlockId)
        ���ڲ��� MFSPartition �����Ϸ���һ���豸�鲢���������һ��������β����
        @param firstBlockId Ҫ��β�������豸��Ŀ����ĵ�һ�����š�
        @return �·���Ŀ��š�������ʧ�ܣ����� MFSBlockAllocationBitmap::InvalidBlockId��

    DWORD MFSPartition::Internals::AllocateFrontBlock(DWORD firstBlockId)
        ���ڲ��� MFSPartition �����Ϸ���һ���豸�鲢���������һ��������ͷ����
        @param firstBlockId Ҫ��ͷ�������豸��Ŀ����ĵ�һ�����š�
        @retrun �·���Ŀ��š�������ʧ�ܣ����� MFSBlockAllocationBitmap::InvalidBlockId��

    DWORD MFSPartition::Internals::AllocateBlockChain(DWORD numberOfBlocks)
        ���ڲ��� MFSPartition �����Ϸ���һ������ָ��������Ŀ�����
        @param numberOfBlocks Ҫ����Ŀ����п�ĸ�����
        @return ������Ŀ����ĵ�һ���š�
        ���޷�����ָ���������豸�飬���� MFSBlockAllocationBitmap::InvalidBlockId��

    DWORD MFSPartition::Internals::FreeChainedBlock(DWORD firstBlockId, DWORD blockId)
        ���ڲ��� MFSPartition �������ͷſ����ϵ�һ�����ݿ顣
        @param firstBlockId �����ĵ�һ���豸���š�
        @param blockId Ҫ�ͷŵĿ�ı�š�
        @return �ͷŲ�����ɺ�������׿��š�

    DWORD MFSPartition::Internals::FreeBlockAfter(DWORD position)
        ���ڲ��� MFSPartition �������ͷŸ������ڿ����ϵ���һ���豸�顣
        @param position �� position �ڿ����ϵ���һ���豸�齫���ͷš�
        @return ���ͷŵĿ��š�

    DWORD MFSPartition::Internals::AppendTailBlock(DWORD firstBlockId, DWORD blockId)
        �������Ŀ�����������Ŀ���ĩβ��
        @param firstBlockId �����ĵ�һ���š�
        @param blockId Ҫ��ӵ�����ĩβ�Ŀ��š�
        @return ��������������ĵ�һ���š�

    DWORD GetNextChainedBlock(DWORD blockId) const
        ��ȡ�ڲ� MFSPartition ������ָ���豸���ڿ����ϵ���һ���š�

    DWORD MFSPartition::Internals::GetAvailableFSNodeId()
        ���ڲ��� MFSPartition �����ϻ�ȡ��һ�����õ��ļ�ϵͳ�ڵ�ı�š�
        ��û�п��õ��ļ�ϵͳ�ڵ㣬���� MFSFSNodePool::InvalidFSNodeId��

    bool MFSPartition::Internals::AllocateEntryMeta(DWORD fsnodeId)
        ���ڲ��� MFSPartition �����ϵ���һ���ļ�ϵͳ�ڵ�Ԫ���ݽṹ�����ü�����
        ���صĽڵ��Ѿ�����֤��ʼ����
        @param fsnodeId Ҫ����Ľڵ��ڽڵ���еı�š�
        @return һ�� bool ֵָʾĿ��ڵ��ڲ���֮ǰ�����ü����Ƿ�Ϊ 0��

    MFSFSEntryMeta * GetEntryMeta(uint32_t fsnodeId) const
        ��ȡ���Ϊ����ֵ�Ľڵ�Ԫ���ݡ�

    bool FreeEntryMeta(uint32_t fsnodeId)
        �ݼ��������ļ�ϵͳ�ڵ�����ü����������ü���������ʱ�ͷ������ӵ��Ŀ�����

    MFSBlockStream * MFSPartition::Internals::OpenBlockStream(DWORD firstBlock)
        ���ڲ��� MFSPartition �����ϴ�һ����������֯��������������ĳ��Ƚ��ᱻ���뵽��߽硣
        @param firstBlock �򿪵�������Ļ��������ĵ�һ���š�

    MFSBlockStream * MFSPartition::Internals::OpenBlockStream(DWORD firstBlock, UINT64 length)
        ���ڲ��� MFSPartition �����ϴ�һ����������֯��������
        @param firstBlock �򿪵�������Ļ��������ĵ�һ���š�
        @param length �򿪵�������ĳ��ȡ�

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
��ʾ�ļ�ϵͳ����һ���ڵ㡣

    ��������

    MFSFSEntry::MFSFSEntry(MFSPartition * partition, uint32_t fsnodeId)
        �Ӹ������ļ�ϵͳ�����Լ��ļ�ϵͳ��ų�ʼ�� MFSFSEntry �����ʵ����

    ��Ա������

    MFSPartition * MFSFSEntry::GetPartition() const
        ��ȡ��ǰ MFSFSEntry ���������Ķ���ʵ����

    uint32_t MFSFSEntry::GetFSNodeId() const
        ��ȡ��ǰ MFSFSEntry ��Ӧ���ļ�ϵͳ�ڵ��š�

    MFSFSEntryType MFSFSEntry::GetEntryType() const
        ��ȡ��ǰ MFSFSEntry ����ʾ���ļ�ϵͳ��Ŀ�����͡�

    void MFSFSEntry::SetEntryType(MFSFSEntryType type)
        ���õ�ǰ MFSFSEntry ����ʾ���ļ�ϵͳ��Ŀ�����͡�

    MFSDateTime MFSFSEntry::GetCreationTime() const
        ��ȡ��ǰ�ļ�ϵͳ�ڵ�Ĵ���ʱ�䡣

    MFSDateTime MFSFSEntry::GetLastAccessTime() const
        ��ȡ��ǰ�ļ�ϵͳ�ڵ����һ�η���ʱ�䡣

    MFSDateTime MFSFSEntry::GetLastModificationTime() const
        ��ȡ��ǰ�ļ�ϵͳ�ڵ���һ�α��޸ĵ�ʱ�䡣

    bool MFSFSEntry::GetHiddenFlag() const
        ��ȡ�ļ�ϵͳ�ڵ������λ��

    bool MFSFSEntry::GetProtectedFlag() const
        ��ȡ�ļ�ϵͳ�ڵ�ı���λ��

    MFSFSEntryAccess MFSFSEntry::GetAccessAttribute() const
        ��ȡ�ļ�ϵͳ�ڵ�ķ��ʿ������ԡ�

    void MFSFSEntry::SetHiddenFlag(bool isHidden)
        �����ļ�ϵͳ�ڵ������λ��

    void MFSFSEntry::SetProtectedFlag(bool isProtected)
        �����ļ�ϵͳ�ڵ�ı���λ��

    void MFSFSEntry::SetAccessAttributes(MFSFSEntryAccess access)
        �����ļ�ϵͳ�ڵ�ķ��ʿ������ԡ�

    uint64_t MFSFSEntry::GetFileSize() const
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ���ļ�ʱ�����ظ��ļ����ֽڴ�С�����򷵻� 0��

    bool MFSFSEntry::SetFileSize(uint64_t size)
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ���ļ�ʱ�����ø��ļ����ֽڴ�С�������õ�ֵ�����ļ������д�С��
        ������¿ռ�ᱻ��������ļ��������õ�ֵС���ļ������д�С���ļ��ᱻ�ضϡ�
        @return һ�� bool ֵָʾ�����Ƿ�ɹ���

    MFSStream * MFSFSEntry::OpenDataStream()
        ���ļ�ϵͳ��Ŀ����������

    std::vector<std::unique_ptr<MFSFSEntry>> MFSFSEntry::GetSubEntries()
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ����ȡ��Ŀ¼���������ļ�ϵͳ�����

    MFSFSEntry * MFSFSEntry::GetSubEntry(const MFSString & name)
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ����ȡ��Ŀ¼��ָ�����Ƶ����ļ�ϵͳ�������Ŀ¼��û��ָ�������ƣ����� nullptr��

    MFSFSEntry * MFSFSEntry::AddSubEntry(const MFSString & name)
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ��������Ŀ¼�´���ָ�����Ƶ����ļ�ϵͳ�����
        ������ʧ�ܻ�ָ���������Ѿ����ڣ����� nullptr��

    MFSFSEntry * MFSFSEntry::AddSubEntry(const MFSString & name, uint32_t fsnodeId)
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ��������Ŀ¼�´����������Ƶ�Ŀ¼�����Ӳ���ӵ�
        �������ļ�ϵͳ�ڵ��ϡ�

    MFSFSEntry * MFSFSEntry::RemoveSubEntry(const MFSString & name)
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ��������Ŀ¼��ɾ��ָ�����Ƶ����ļ�ϵͳ�����
        @return ����һ�� bool ֵָʾɾ���Ƿ�ɹ���

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

