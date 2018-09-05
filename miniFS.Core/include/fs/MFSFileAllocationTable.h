#pragma once

#include <cstdint>
#include <vector>


/*

class MFSFileAllocationTable
���ļ�������ṩ����

    ��������

    MFSFileAllocationTable::MFSFileAllocationTable(uint32_t itemsCount)
        ��ʼ�� MFSFileAllocationTable �����ʵ����
        @param itemsCount FAT�а����Ŀ�������

    ��Ա������

    uint32_t MFSFileAllocationTable::Get(uint32_t pos) const
        ǿ�ƻ�ȡ FAT ��ָ��λ�ô���ֵ��

    void MFSFileAllocationTable::Set(uint32_t pos, uint32_t value)
        ǿ������ FAT ��ָ��λ�ô���ֵ��

    uint32_t MFSFileAllocationTable::GetCount() const
        ��ȡ FAT �еĿ�������

    uint32_t MFSFileAllocationTable::GetLength(uint32_t first) const
        ��ȡĳ�������ĳ��ȡ�
        @param �����ĵ�һ���š�

    uint32_t MFSFileAllocationTable::GetTailBlock(uint32_t first) const
        ��ȡ�����Ŀ��������һ�����š�
        @param first �����ĵ�һ�����š�

    uint32_t MFSFileAllocationTable::GetBlockBefore(uint32_t first, uint32_t blockId) const
        ��ȡ�����Ŀ����и������ǰһ����Ŀ��š�
        @param first �����ĵ�һ�����š�
        @param blockId ���صĿ���Ϊ������ڿ����е�ǰһ���顣

    void MFSFileAllocationTable::AddAfter(uint32_t position, uint32_t blockId)
        �������Ŀ���������һ�����Ŀ�󲿡�
        @param position blockId ��ָʾ�Ŀ齫�����ӵ������󲿡�
        @param blockId Ҫ���ӵ� position ��ָʾ�Ŀ�󲿵Ŀ� ID��

    void MFSFileAllocationTable::AddTail(uint32_t first, uint32_t blockId)
        �������Ŀ���������һ�����Ŀ������Ŀ�����ĩβ��
        @param first �����ĵ�һ�����š�
        @param blockId Ҫ���ӵ�ָ���Ŀ���ĩβ�Ŀ��š�

    void MFSFileAllocationTable::AddFront(uint32_t first, uint32_t blockId)
        �������Ŀ������������Ŀ����Ŀ�ͷ��
        @param first �����ĵ�һ�����š�
        @param blockId Ҫ���ӵ�ָ���Ŀ�����ͷ�Ŀ��š�

    int MFSFileAllocationTable::RemoveAfter(uint32_t position)
        �������Ŀ�ĺ�һ����ӿ������Ƴ���
        @return ���Ƴ��Ŀ��š�
    
    uint32_t MFSFileAllocationTable::Remove(uint32_t first, uint32_t blockId)
        �Ƴ������Ŀ����е�ĳһ�顣
        @param first Ҫɾ���Ŀ������Ŀ����ĵ�һ���š�
        @param blockId Ҫɾ���Ŀ�ı�š�
        @return ����Ƴ�������Ŀ����ĵ�һ���š�

    uint32_t MFSFileAllocationTable::RemoveFront(uint32_t first)
        �Ƴ������Ŀ����ĵ�һ�顣
        @param first Ҫɾ���׿�Ŀ����ĵ�һ���š�
        @return �׿鱻ɾ��������ĵ�һ���š�

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
