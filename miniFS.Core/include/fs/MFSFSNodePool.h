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

    uint32_t GetAvailableFSNodeId()
        获取节点池中下一个可用的文件系统节点编号。若没有可用的编号，返回 InvalidFSNodeId。

    bool Allocate(uint32_t fsnodeId)
        递增节点池中编号为 fsnodeId 的节点的引用计数。若节点的初始引用计数为 0，则函数返回前会将节点初始化。

    bool Release(uint32_t fsnodeId)
        递减给定文件系统节点的引用计数。若递减后的引用计数值为 0，返回 true；否则返回 false。

*/

class MFSFSNodePool
{
public:
    static constexpr size_t InvalidFSNodeId = 0xFFFFFFFF;

    MFSFSNodePool(size_t numberOfNodes);

    uint32_t GetAvailableFSNodeId();
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

    uint32_t LocateNextFreeNode();
    void InitializeFSNode(uint32_t fsnodeId);

    friend class MFSFSNodePoolSerializer;
};
