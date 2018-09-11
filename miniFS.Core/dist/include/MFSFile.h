#pragma once

#include "../../include/MFSString.h"
#include "../../include/MFSDateTime.h"
#include "../../include/stream/MFSStream.h"

class MFSFSEntry;


/*

class MFSFile
表示一个文件。

    成员函数：

    MFSDateTime MFSFile::GetCreationTime() const noexcept
        获取文件的创建时间。返回的时间已经本地化。

    MFSDateTime MFSFile::GetLastAccessTime() const noexcept
        获取文件的上次访问时间。返回的时间已经本地化。

    MFSDateTime MFSFile::GetLastModificationTime() const noexcept
        获取文件的上次修改时间。返回的时间已经本地化。

    bool MFSFile::IsHidden() const noexcept
        获取一个 bool 值指示文件是否为隐藏文件。

    void MFSFile::SetHidden(bool isHidden) noexcept
        设置文件的隐藏标记。

    uint64_t MFSFile::GetFileSize() const noexcept
        获取文件的字节大小。

    void MFSFile::SetFileSize(uint64_t size)
        设置文件的字节大小。若新大小大于文件的当前大小，则所需的空间将会被尝试分配；若新大小小于文件的当前大小，则文件将会被截断；
        若新大小与文件的当前大小一致，则此函数不会执行任何操作。
        @exception
            MFSOutOfSpaceException 数据空间空间不足，无法完成所需的操作。

    std::vector<uint32_t> MFSFile::GetAllBlocksId() const noexcept
        获取文件所占用的所有数据块的设备块编号。

    MFSStream * MFSFile::OpenStream() noexcept
        打开文件数据流进行读写。

*/

class MFSFile
{
public:
    ~MFSFile();

    uint64_t GetFileSize() const noexcept;
    void SetFileSize(uint64_t size);

    std::vector<uint32_t> GetAllBlocksId() const noexcept;

    MFSStream * OpenStream() noexcept;

private:
	MFSFSEntry * _entry;
    MFSFile(MFSFSEntry * entry);

    friend class MFSDataSpace;
};
