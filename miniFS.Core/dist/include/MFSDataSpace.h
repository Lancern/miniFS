#pragma once

#include "../../include/MFSString.h"
#include "../../include/MFSDateTime.h"
#include "MFSFile.h"
#include <memory>

class MFSOSFileDevice;
class MFSBlockDevice;
class MFSPartition;
class MFSFSEntry;
enum struct MFSFSEntryType;


/*

class MFSDataSpace
提供对数据空间的操作。该类的实例对象上的复制语义已被禁用。

    构造器：

    MFSDataSpace::MFSDataSpace(const MFSString & osFileName)
        从给定的底层文件初始化 MFSDataSpace 类的新实例。
        @param osFileName 底层操作系统文件名。
        @exception
            MFSWindowsException 打开底层操作系统文件时出现错误。
            MFSInvalidDeviceException 作为后备设备的底层操作系统文件不合法。

    成员函数：

    MFSString MFSDataSpace::GetWorkingDirectory() const noexcept
        获取数据空间的当前工作目录。

    void MFSDataSpace::SetWorkingDirectory(const MFSString & path)
        设置数据空间的当前工作目录。
        @exceptions
            MFSInvalidPathException 给定的路径不合法。

    uint64_t MFSDataSpace::GetTotalSpaceInBytes() const noexcept
        获取数据空间的总字节大小。

    uint64_t MFSDataSpace::GetFreeSpaceInBytes() const noexcept
        获取数据空间的总可用大小。

    bool MFSDataSpace::IsFormatted() const noexcept
        获取一个 bool 值指示当前的数据空间是否已经经过格式化。

    void MFSDataSpace::Format() noexcept
        格式化当前的数据空间。

    void MFSDataSpace::Optimize() noexcept
        对当前的数据空间执行碎片整理。

    bool MFSDataSpace::Exist(const MFSString & path)
        检查给定的路径是否存在。
        @exceptions
            MFSInvalidPathException：给定的路径不是一个合法路径。

    MFSFile * MFSDataSpace::OpenFile(const MFSString & path, bool createIfNotExist)
        打开一个文件。
        @param path 文件的路径。
        @param createIfNotExist 当该参数为 true 时，若文件不存在将创建文件；否则将抛出 MFSFileNotFoundException 异常。
        @exceptions
            MFSInvalidPathException 给定的路径不合法。
            MFSDirectoryNotFoundException 路径上有一个或多个目录不存在。
            MFSFileNotFoundException 文件不存在且 createIfNotExist 参数为 false。
            MFSDirectoryAlreadyExistException 已经存在了一个同名目录。

    MFSFile * MFSDataSpace::CreateFile(const MFSString & path, bool openIfExist)
        创建一个文件。
        @param path 文件的路径。
        @param openIfExist 当该参数为 true 时，若文件已经存在将直接打开文件；否则将抛出 MFSFileAlreadyExistException 异常。
        @exceptions
            MFSInvalidPathException 给定的路径不合法。
            MFSDirectoryNotFoundException 路径上有一个或多个目录不存在。
            MFSFileAlreadyExistException 文件已经存在且 openIfExist 参数为 false。
            MFSDirectoryAlreadyExistException 已经存在一个同名的目录。
            MFSOutOfSpaceException 数据空间空间不足，无法完成要求的操作。

    void MFSDataSpace::CreateDirectory(const MFSString & path, bool errorIfExist)
        创建一个目录及其所有不存在的直接或间接父目录。
        @param path 目录的路径。
        @param errorIfExist 当该参数为 false 时，若目录已经存在则本函数不会执行任何操作；否则将抛出 MFSDirectoryAlreadyExistException 异常。
        @exceptions
            MFSInvalidPathException 给定的路径不合法。
            MFSFileAlreadyExist 路径上存在一个或多个同名文件。
            MFSDirectoryAlreadyExist 要创建的目录已存在 且 errorIfExist 为 true。
            MFSOutOfSpaceException 数据空间空间不足，无法完成要求的操作。
            
    void MFSDataSpace::CreateLink(const MFSString & src, const MFSString & target)
        创建一个硬链接。
        @param src 硬链接的源。
        @param target 硬链接的目标。
        @exceptions
            MFSInvalidPathException 目标路径 target 或 源路径link 不合法。
            MFSDirectoryNotFoundException 路径上有一个或多个目录不存在。
            MFSFileAlreadyExist 路径上存在一个或多个同名文件。
            MFSOutOfSpaceException 数据空间空间不足，无法完成要求的操作。

    void MFSDataSpace::Delete(const MFSString & path)
        删除一个目录、文件或硬链接。
        @param path 要删除的目录、文件或硬链接的名称。
        @exceptions
            MFSInvalidPathException 路径不合法。
            MFSDirectoryNotFoundException 路径上有一个或多个目录不存在。
            MFSFileNotFoundException 要删除的目录、文件或硬链接不存在。
            MFSInvalidOperationException 要删除的目标是一个非空目录。

    MFSEntryInfo MFSDataSpace::GetEntryInfo(const MFSString & path)
        获取一个文件或目录的属性信息。
        @param path 文件或目录的路径。
        @exceptions
            MFSInvalidPathException 路径不合法。
            MFSDirectoryNotFoundException 路径上有一个或多个目录不存在。
            MFSFileNotFoundException 目标文件或目录不存在。

    void MFSDataSpace::SetHidden(const MFSString & path, bool isHidden)
        设置一个文件或目录的隐藏标志位。
        @param path 文件或目录的路径。
        @param isHidden 一个 bool 值指示是否隐藏。
        @exceptions
            MFSInvalidPathException 路径不合法。
            MFSDirectoryNotFoundException 路径上有一个或多个目录不存在。
            MFSFileNotFoundException 目标文件或目录不存在。

    void MFSDataSpace::Copy(const MFSString & source, const MFSString & destination)
        复制一个文件。
        @param source 源文件路径。
        @param destination 目标文件路径。
        @exception
            MFSInvalidPathException 源路径 source 或 目标路径 destination 不合法。
            MFSDirectoryNotFoundException 源路径或目标路径上的一个或多个目录不存在。
            MFSFileNotFoundException 要复制的源文件或目标文件不存在。
            MFSInvalidOperationException source 所标识的文件系统项不是一个文件。
            MFSOutOfSpaceException 数据空间空间不足，无法完成要求的操作。

    void MFSDataSpace::Move(const MFSString & source, const MFSString & destination)
        移动一个目录或文件。
        @param source 源路径。
        @param destination 目标路径。
        @exception
            MFSInvalidPathException 源路径 source 或 目标路径 destination 不合法。
            MFSDirectoryNotFoundException 源路径或目标路径上的一个或多个目录不存在。
			MFSFileAlreadyExistException 目标文件已存在。
            MFSFileNotFoundException 要移动的源文件不存在。
			MFSOutOfSpaceException 数据空间空间不足，无法完成要求的操作。

    std::vector<MFSString> MFSDataSpace::GetDirectories(const MFSString & directory)
        获取某个目录下的所有子目录名称。
        @param directory 要获取所有子目录的目录路径。
        @exception
            MFSInvalidPathException 给定的路径不合法。
            MFSDirectoryNotFoundException 给定的路径上有一个或多个目录不存在。

    void MFSDataSpace::GetFiles(const MFSString & directory)
        获取某个目录下的所有子文件名称。
        @param directory 要获取所有子文件的目录路径。
        @exception
            MFSInvalidPathException 给定的路径不合法。
            MFSDirectoryNotFoundException 给定的路径上有一个或多个目录不存在。

    void MFSDataSpace::Close() noexcept
        关闭当前的数据空间对象并释放其占用的所有外部资源。


    静态成员函数：

    static MFSDataSpace * MFSDataSpace::CreateDataSpace(const MFSString & filename, uint64_t size)
        创建一个数据空间。
        @param filename 数据空间所对应的底层操作系统文件名。
        @param size 数据空间的总字节大小。该参数不应该小于 134,217,728，即 128 MB；同时不应该大于 4,294,967,296，即 4GB。
                    该参数在函数内部将会被向上对齐到 4KB 的整数倍。
        @exceptions
            MFSInvalidArgumentException size 过小或过大。
            MFSWindowsException 在与操作系统交互过程中出现错误。

    static MFSDataSpace * MFSDataSpace::GetActiveDataSpace() noexcept
        获取应用程序中当前活动的数据空间。若当前应用程序中没有活动的数据空间，返回 nullptr。

    static void MFSDataSpace::SetActiveDataSpace(MFSDataSpace * dataSpace) noexcept
        设置应用程序中的活动的数据空间。此操作不转让传入的指针的所有权。

*/

struct MFSEntryInfo
{
    MFSDateTime CreationTime;
    MFSDateTime LastAccessTime;
    MFSDateTime LastModificationTime;
    bool IsDirectory;
    bool IsHidden;
};

#ifdef CreateFile
// Undef Windows-defined macro CreateFile for it conflicts with the MFSDataSpace::CreateFile.
#undef CreateFile
#endif

#ifdef CreateDirectory
// Undef Windows-defined macro CreateFile for it conflicts with the MFSDataSpace::CreateDirectory.
#undef CreateDirectory
#endif

class MFSDataSpace
{
public:
    MFSDataSpace(const MFSString & osFileName);

    MFSDataSpace(const MFSDataSpace &) = delete;
    MFSDataSpace(MFSDataSpace && another);

    MFSDataSpace & operator = (const MFSDataSpace &) = delete;
    MFSDataSpace & operator = (MFSDataSpace && another);

    MFSString GetWorkingDirectory() const noexcept;
    void SetWorkingDirectory(const MFSString & path);

    uint64_t GetTotalSpaceInBytes() const noexcept;
    uint64_t GetFreeSpaceInBytes() const noexcept;

    bool IsFormatted() const noexcept;
    
    void Format() noexcept;
    void Optimize() noexcept;

    bool Exist(const MFSString & path);
    MFSFile * OpenFile(const MFSString & path, bool createIfNotExist);
    MFSFile * CreateFile(const MFSString & path, bool openIfExist);
    void CreateDirectory(const MFSString & path, bool errorIfExist);
    void CreateLink(const MFSString & src, const MFSString & target);
    void Delete(const MFSString & path);

    MFSEntryInfo GetEntryInfo(const MFSString & path);
    void SetHidden(const MFSString & path, bool isHidden);

    void Copy(const MFSString & source, const MFSString & destination);
    void Move(const MFSString & source, const MFSString & destination);

    std::vector<MFSString> GetDirectories(const MFSString & directory);
    std::vector<MFSString> GetFiles(const MFSString & directory);

    void Close() noexcept;

    static MFSDataSpace * CreateDataSpace(const MFSString & filename, uint64_t size);
    
    static MFSDataSpace * GetActiveDataSpace() noexcept;
    static void SetActiveDataSpace(MFSDataSpace * dataSpace) noexcept;

private:
    MFSString _workingDirectory;

    HANDLE _hFile;
    std::shared_ptr<MFSOSFileDevice> _fileDevice;
    std::shared_ptr<MFSBlockDevice> _blockDevice;
    // NOTICE: MFSPartition is an incomplete type and yet we should use std::shared_ptr as below shows.
    std::shared_ptr<MFSPartition> _partition;

    MFSFSEntry * OpenRootFSEntry();
    MFSFSEntry * OpenFSEntry(const MFSString & path);

    std::vector<MFSString> GetSubEntryNames(const MFSString & directory, MFSFSEntryType entryType);
};
