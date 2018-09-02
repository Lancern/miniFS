#pragma once

#include "MFSMetas.h"
#include <cstdint>
#include <memory>



/*

class MFSFSNodePool
表示文件系统节点池。
    
    构造器：

    MFSFSNodePool::MFSFSNodePool(size_t numberOfNodes)
        从节点池中的节点数量初始化 MFSFSNodePool 类的新实例。

    uint32_t MFSFSNodePool::Allocate()
        尝试从节点池中分配一个文件系统节点并返回分配的节点的编号。若分配失败，返回 MFSFSNodePool::InvalidFSNodeId。

    bool Allocate(uint32_t fsnodeId)
        尝试从节点池中分配编号为 fsnodeId 的节点并返回一个值表示分配是否成功。

    bool Release(uint32_t fsnodeId)
        递减给定文件系统节点的引用计数。若递减后的引用计数值为 0，返回 true；否则返回 false。

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
