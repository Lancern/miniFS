#pragma once

#include "MFSPartition.h"
#include "MFSMetas.h"
#include "../stream/MFSStream.h"
#include "../MFSString.h"
#include "../MFSDateTime.h"
#include "../serialize/MFSDirectoryBlockSerializer.h"
#include <vector>
#include <memory>


/*

class MFSFSEntry
��ʾ�ļ�ϵͳ����һ���ڵ㡣

    ��������

    MFSFSEntry::MFSFSEntry(MFSPartition * partition, uint32_t fsnodeId)
        �Ӹ������ļ�ϵͳ�����Լ��ļ�ϵͳ��ų�ʼ�� MFSFSEntry �����ʵ����

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

    MFSStream * OpenDataStream()
        ���ļ�ϵͳ��Ŀ����������

    std::vector<std::unique_ptr<MFSFSEntry>> GetSubEntries()
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ����ȡ��Ŀ¼���������ļ�ϵͳ�����

    MFSFSEntry * GetSubEntry(const MFSString & name)
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ����ȡ��Ŀ¼��ָ�����Ƶ����ļ�ϵͳ�������Ŀ¼��û��ָ�������ƣ����� nullptr��

    MFSFSEntry * AddSubEntry(const MFSString & name)
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ��������Ŀ¼�´���ָ�����Ƶ����ļ�ϵͳ�����������ʧ�ܻ�ָ���������Ѿ����ڣ����� nullptr��

    MFSFSEntry * RemoveSubEntry(const MFSString & name)
        ���ļ�ϵͳ�ڵ�����ʾ���ļ�ϵͳ��Ϊһ��Ŀ¼ʱ��������Ŀ¼��ɾ��ָ�����Ƶ����ļ�ϵͳ�������ɾ���ɹ�������ɾ��������ļ�ϵͳ��ʵ������
        ���򷵻� nullptr��

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
    MFSFSEntry(MFSPartition * partition, uint32_t fsnodeId);

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

    MFSStream * OpenDataStream();

    auto GetSubEntries() -> std::vector<std::pair<MFSString, std::unique_ptr<MFSFSEntry>>>;
	bool ContainsSubEntry(const MFSString & name);
    MFSFSEntry * GetSubEntry(const MFSString & name);
    MFSFSEntry * AddSubEntry(const MFSString & name);
    MFSFSEntry * RemoveSubEntry(const MFSString & name);

private:
    MFSPartition * _partition;
    MFSFSEntryMeta * _meta;
    uint32_t _fsnodeId;

	template <typename Callback>
	void WalkDirectoryBlocks(Callback callback);
};

template<typename Callback>
inline void MFSFSEntry::WalkDirectoryBlocks(Callback callback)
{
	std::unique_ptr<MFSStream> stream(OpenDataStream());
	MFSDirectoryBlockSerializer serializer(_partition->GetDevice()->GetBlockSize());
	while (stream->HasNext())
	{
		std::unique_ptr<MFSDirectoryBlock> block(serializer.Deserialize(stream.get()));
		if (!callback(block.get()))
			break;
	}
	stream->Close();
	return ret;
}
