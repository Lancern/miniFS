#pragma once

#include "../device/MFSBlockDevice.h"
#include "../stream/MFSBlockStream.h"
#include "MFSFSEntry.h"
#include "MFSMetas.h"
#include "MFSBlockAllocationManager.h"
#include "MFSAllocationTable.h"
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
    std::unique_ptr<MFSBlockDevice> _device;
    MFSFSMasterInfo _master;
    std::unique_ptr<MFSAllocationTable> _blockChain;
    std::unique_ptr<MFSBlockAllocationManager> _blockAllocation;
    std::unique_ptr<MFSFSEntryMeta[]> _fsnodePool;
    bool _validDevice;
    
    void LoadDevice(MFSBlockDevice * device);
    bool LoadMasterInfo(MFSBlockStream * deviceStream);
    bool LoadBlockAllocationManager(MFSBlockStream * deviceStream);
    bool LoadAllocationTable(MFSBlockStream * deviceStream);
    bool LoadFSNodePool(MFSBlockStream * deviceStream);
};
