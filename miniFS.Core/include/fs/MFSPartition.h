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

*/

class MFSPartition
{
public:
    MFSPartition(MFSBlockDevice * device);
    ~MFSPartition();

    MFSBlockDevice * GetDevice() const;
    bool IsValidDevice() const;

    bool IsRaw() const;
    UINT64 GetTotalSpaceInBytes() const;
    UINT64 GetFreeSpaceInBytes() const;
    
    void BuildFileSystem();

    MFSFSEntry * GetRoot() const;

    void Flush();
    void Close();

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

    ChainedBlockStream * OpenBlockStream(DWORD firstBlock);
    ChainedBlockStream * OpenBlockStream(DWORD firstBlock, UINT64 length);

    MFSFSEntryMeta * GetEntryMeta(uint32_t fsnodeId) const;

    friend class MFSFSEntry;
};
