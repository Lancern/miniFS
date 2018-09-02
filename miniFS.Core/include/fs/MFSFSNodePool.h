#pragma once

#include "MFSMetas.h"
#include <cstdint>
#include <memory>



/*

class MFSFSNodePool
��ʾ�ļ�ϵͳ�ڵ�ء�
    
    ��������

    MFSFSNodePool::MFSFSNodePool(size_t numberOfNodes)
        �ӽڵ���еĽڵ�������ʼ�� MFSFSNodePool �����ʵ����

    uint32_t MFSFSNodePool::Allocate()
        ���Դӽڵ���з���һ���ļ�ϵͳ�ڵ㲢���ط���Ľڵ�ı�š�������ʧ�ܣ����� MFSFSNodePool::InvalidFSNodeId��

    bool Allocate(uint32_t fsnodeId)
        ���Դӽڵ���з�����Ϊ fsnodeId �Ľڵ㲢����һ��ֵ��ʾ�����Ƿ�ɹ���

    bool Release(uint32_t fsnodeId)
        �ݼ������ļ�ϵͳ�ڵ�����ü��������ݼ�������ü���ֵΪ 0������ true�����򷵻� false��

*/

class MFSFSNodePool
{
public:
    static constexpr size_t InvalidFSNodeId = 0xFFFFFFFF;

    MFSFSNodePool(size_t numberOfNodes);

    uint32_t Allocate();
    bool Allocate(uint32_t fsnodeId);
    bool Release(uint32_t fsnodeId);

    MFSFSEntryMeta & operator [] (uint32_t fsnodeId);
    MFSFSEntryMeta operator [] (uint32_t fsnodeId) const;

    MFSFSEntryMeta & Get(uint32_t fsnodeId);
    MFSFSEntryMeta Get(uint32_t fsnodeId) const;

private:
    std::unique_ptr<MFSFSEntryMeta[]> _pool;
    size_t _nodesCount;
    size_t _alloc;

    size_t LocateNextFreeNode();

    friend class MFSFSNodePoolSerializer;
};
