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
表示文件系统树的一个节点。

    构造器：

    MFSFSEntry::MFSFSEntry(MFSPartition * partition, uint32_t fsnodeId)
        从给定的文件系统分区以及文件系统编号初始化 MFSFSEntry 类的新实例。

    MFSPartition * MFSFSEntry::GetPartition() const
        获取当前 MFSFSEntry 所属分区的对象实例。

    uint32_t MFSFSEntry::GetFSNodeId() const
        获取当前 MFSFSEntry 对应的文件系统节点编号。

    MFSFSEntryType MFSFSEntry::GetEntryType() const
        获取当前 MFSFSEntry 所表示的文件系统项目的类型。

    void MFSFSEntry::SetEntryType(MFSFSEntryType type)
        设置当前 MFSFSEntry 所表示的文件系统项目的类型。

    MFSDateTime MFSFSEntry::GetCreationTime() const
        获取当前文件系统节点的创建时间。

    MFSDateTime MFSFSEntry::GetLastAccessTime() const
        获取当前文件系统节点的上一次访问时间。

    MFSDateTime MFSFSEntry::GetLastModificationTime() const
        获取当前文件系统节点上一次被修改的时间。

    bool MFSFSEntry::GetHiddenFlag() const
        获取文件系统节点的隐藏位。

    bool MFSFSEntry::GetProtectedFlag() const
        获取文件系统节点的保护位。

    MFSFSEntryAccess MFSFSEntry::GetAccessAttribute() const
        获取文件系统节点的访问控制属性。

    void MFSFSEntry::SetHiddenFlag(bool isHidden)
        设置文件系统节点的隐藏位。

    void MFSFSEntry::SetProtectedFlag(bool isProtected)
        设置文件系统节点的保护位。

    void MFSFSEntry::SetAccessAttributes(MFSFSEntryAccess access)
        设置文件系统节点的访问控制属性。

    uint64_t MFSFSEntry::GetFileSize() const
        当文件系统节点所表示的文件系统项为一个文件时，返回该文件的字节大小；否则返回 0。

    MFSStream * OpenDataStream()
        打开文件系统项目数据流对象。

    std::vector<std::unique_ptr<MFSFSEntry>> GetSubEntries()
        当文件系统节点所表示的文件系统项为一个目录时，获取该目录下所有子文件系统项对象。

    MFSFSEntry * GetSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，获取该目录下指定名称的子文件系统项对象。若目录下没有指定的名称，返回 nullptr。

    MFSFSEntry * AddSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，尝试在目录下创建指定名称的子文件系统项对象。若创建失败或指定的名称已经存在，返回 nullptr。

    MFSFSEntry * RemoveSubEntry(const MFSString & name)
        当文件系统节点所表示的文件系统项为一个目录时，尝试在目录下删除指定名称的子文件系统项对象。若删除成功，返回删除的项的文件系统项实例对象；
        否则返回 nullptr。

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
