#pragma once

#include "MFSMetas.h"
#include <cstdint>
#include <memory>



/*

class MFSFSNodePool
��ʾ�ļ�ϵͳ�ڵ�ء�
    
    ��������

    MFSFSNodePool::MFSFSNodePool(uint32_t numberOfNodes)
        �ӽڵ���еĽڵ�������ʼ�� MFSFSNodePool �����ʵ����

    uint32_t GetNextAvailableFSNodeId()
        ��ȡ�ڵ������һ�����õ��ļ�ϵͳ�ڵ��š���û�п��õı�ţ����� InvalidFSNodeId��

    bool Allocate(uint32_t fsnodeId)
        �����ڵ���б��Ϊ fsnodeId �Ľڵ�����ü��������ڵ�ĳ�ʼ���ü���Ϊ 0����������ǰ�Ὣ�ڵ��ʼ����

    bool Release(uint32_t fsnodeId)
        �ݼ������ļ�ϵͳ�ڵ�����ü��������ݼ�������ü���ֵΪ 0������ true�����򷵻� false��

*/

class MFSFSNodePool
{
public:
    class Iterator
    {
    public:
        using difference_type = size_t;
        using value_type = MFSFSEntryMeta;
        using pointer = MFSFSEntryMeta *;
        using reference = MFSFSEntryMeta &;
        using iterator_category = std::bidirectional_iterator_tag;

        Iterator & operator ++ ();
        Iterator operator ++ (int);
        Iterator & operator -- ();
        Iterator operator -- (int);

        bool operator == (const Iterator & another) const;
        bool operator != (const Iterator & another) const;

        value_type operator * () const;
        reference operator * ();
        pointer operator -> ();

    private:
        Iterator(MFSFSNodePool * container, int64_t offset);

        MFSFSNodePool * _container;
        int64_t _offset;

        void LocateNextUsedNode();
        void LocatePreviousUsedNode();

        friend class MFSFSNodePool;
    };

    static constexpr uint32_t InvalidFSNodeId = 0xFFFFFFFF;

    MFSFSNodePool(uint32_t numberOfNodes);

    uint32_t GetNodesCount() const;

    uint32_t GetAvailableFSNodeId();
    bool Allocate(uint32_t fsnodeId);
    bool Release(uint32_t fsnodeId);

    MFSFSEntryMeta & operator [] (uint32_t fsnodeId);
    MFSFSEntryMeta operator [] (uint32_t fsnodeId) const;

    MFSFSEntryMeta & Get(uint32_t fsnodeId);
    MFSFSEntryMeta Get(uint32_t fsnodeId) const;

    Iterator begin();
    Iterator end();

private:
    std::unique_ptr<MFSFSEntryMeta[]> _pool;
    uint32_t _nodesCount;
    uint32_t _alloc;

    uint32_t LocateNextFreeNode();
    void InitializeFSNode(uint32_t fsnodeId);

    friend class MFSFSNodePoolSerializer;
};
