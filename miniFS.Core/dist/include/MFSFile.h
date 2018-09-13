#pragma once

#include "../../include/MFSString.h"
#include "../../include/MFSDateTime.h"
#include "../../include/stream/MFSStream.h"

class MFSFSEntry;


/*

class MFSFile
表示一个文件。

    成员函数：

    MFSString GetFileName() const noexcept
        获取文件名。

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

    MFSString GetFileName() const noexcept;

    uint64_t GetFileSize() const noexcept;
    void SetFileSize(uint64_t size);

    std::vector<uint32_t> GetAllBlocksId() const noexcept;

    MFSStream * OpenStream() noexcept;

private:
	MFSFSEntry * _entry;
    MFSString _filename;

    MFSFile(MFSFSEntry * entry, const MFSString & filename);

    friend class MFSDataSpace;
};
