#pragma once

#include <cstdint>
#include <vector>


/*

class MFSFileAllocationTable
对文件分配表提供抽象。

    构造器：

    MFSFileAllocationTable::MFSFileAllocationTable(uint32_t itemsCount)
        初始化 MFSFileAllocationTable 类的新实例。
        @param itemsCount FAT中包含的块数量。

    成员函数：

    uint32_t MFSFileAllocationTable::Get(uint32_t pos) const
        强制获取 FAT 中指定位置处的值。

    void MFSFileAllocationTable::Set(uint32_t pos, uint32_t value)
        强制设置 FAT 中指定位置处的值。

    uint32_t MFSFileAllocationTable::GetCount() const
        获取 FAT 中的块数量。

    uint32_t MFSFileAllocationTable::GetLength(uint32_t first) const
        获取某个块链的长度。
        @param 块链的第一块编号。

    uint32_t MFSFileAllocationTable::GetTailBlock(uint32_t first) const
        获取给定的块链的最后一个块编号。
        @param first 块链的第一个块编号。

    uint32_t MFSFileAllocationTable::GetBlockBefore(uint32_t first, uint32_t blockId) const
        获取给定的块链中给定块的前一个块的块编号。
        @param first 块链的第一个块编号。
        @param blockId 返回的块编号为这个块在块链中的前一个块。

    void MFSFileAllocationTable::AddAfter(uint32_t position, uint32_t blockId)
        将给定的块链接至另一给定的块后部。
        @param position blockId 所指示的块将被链接到这个块后部。
        @param blockId 要链接到 position 所指示的块后部的块 ID。

    void MFSFileAllocationTable::AddTail(uint32_t first, uint32_t blockId)
        将给定的块链接至另一给定的块所属的块链的末尾。
        @param first 块链的第一个块编号。
        @param blockId 要链接到指定的块链末尾的块编号。

    void MFSFileAllocationTable::AddFront(uint32_t first, uint32_t blockId)
        将给定的块链接至给定的块链的开头。
        @param first 块链的第一个块编号。
        @param blockId 要链接到指定的块链开头的块编号。

    int MFSFileAllocationTable::RemoveAfter(uint32_t position)
        将给定的块的后一个块从块链中移除。
        @return 被移除的块编号。
    
    uint32_t MFSFileAllocationTable::Remove(uint32_t first, uint32_t blockId)
        移除给定的块链中的某一块。
        @param first 要删除的块所属的块链的第一块编号。
        @param blockId 要删除的块的编号。
        @return 完成移除操作后的块链的第一块编号。

    uint32_t MFSFileAllocationTable::RemoveFront(uint32_t first)
        移除给定的块链的第一块。
        @param first 要删除首块的块链的第一块编号。
        @return 首块被删除后块链的第一块编号。

*/

class MFSFileAllocationTable
{
public:
    static constexpr uint32_t InvalidBlockId = 0xFFFFFFFF;

	MFSFileAllocationTable(uint32_t itemsCount);
	~MFSFileAllocationTable();

	uint32_t & operator [] (uint32_t pos);
	uint32_t operator [] (uint32_t pos) const;

    uint32_t Get(uint32_t pos) const;
    void Set(uint32_t pos, uint32_t value);

    uint32_t GetCount() const;
    uint32_t GetLength(uint32_t first) const;

    uint32_t GetTailBlock(uint32_t first) const;
    uint32_t GetBlockBefore(uint32_t first, uint32_t blockId) const;

	void AddAfter(uint32_t position, uint32_t blockId);
    void AddTail(uint32_t first, uint32_t blockId);
    void AddFront(uint32_t first, uint32_t blockId);

    uint32_t RemoveAfter(uint32_t position);
    uint32_t Remove(uint32_t first, uint32_t blockId);
    uint32_t RemoveFront(uint32_t first);

	friend class MFSFileAllocationTableSerializer;

private:
	std::vector<uint32_t> _next;
};
