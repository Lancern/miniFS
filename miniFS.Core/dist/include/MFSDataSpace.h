#pragma once

#include "../../include/MFSString.h"
#include "MFSFile.h"
#include <memory>

class MFSPartition;


/*

class MFSDataSpace
提供对数据空间的操作。

    构造器：

    MFSDataSpace::MFSDataSpace(const MFSString & osFileName)
        从给定的底层文件初始化 MFSDataSpace 类的新实例。
        @param osFileName 底层操作系统文件名。

    成员函数：

    MFSString MFSDataSpace::GetWorkingDirectory() const
        获取数据空间的当前工作目录。

    void MFSDataSpace::SetWorkingDirectory() const
        设置数据空间的当前工作目录。

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

    MFSFile * MFSDataSpace::CreateFile(const MFSString & path, bool openIfExist)
        创建一个文件。
        @param path 文件的路径。
        @param openIfExist 当该参数为 true 时，若文件已经存在将直接打开文件；否则将抛出 MFSFileAlreadyExistException 异常。
        @exceptions
            MFSInvalidPathException 给定的路径不合法。
            MFSDirectoryNotFoundException 路径上有一个或多个目录不存在。
            MFSFileAlreadyExistException 文件已经存在且 openIfExist 参数为 false。
            MFSOutOfSpaceException 数据空间空间不足，无法完成要求的操作。

    void MFSDataSpace::CreateDirectory(const MFSString & path)
        创建一个目录及其所有不存在的直接或间接父目录。
        @param path 目录的路径。
        @exceptions
            MFSInvalidPathException 给定的路径不合法。
            MFSFileAlreadyExist 路径上存在一个或多个同名文件。
            MFSOutOfSpaceException 数据空间空间不足，无法完成要求的操作。
            
    void MFSDataSpace::CreateLink(const MFSString & target, const MFSString & link)
        创建一个硬链接。
        @param target 硬链接的目标。
        @param link 硬链接的源。
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

    void MFSDataSpace::Copy(const MFSString & source, const MFSString & destination)
        复制一个目录或文件。
        @param source 源路径。
        @param destination 目标路径。
        @exception
            MFSInvalidPathException 源路径 source 或 目标路径 destination 不合法。
            MFSDirectoryNotFoundException 源路径或目标路径上的一个或多个目录不存在。
            MFSFileNotFoundException 要复制的源文件或目标文件不存在。
            MFSOutOfSpaceException 数据空间空间不足，无法完成要求的操作。

    void MFSDataSpace::Move(const MFSString & source, const MFSString & destination)
        移动一个目录或文件。
        @param source 源路径。
        @param destination 目标路径。
        @exception
            MFSInvalidPathException 源路径 source 或 目标路径 destination 不合法。
            MFSDirectoryNotFoundException 源路径或目标路径上的一个或多个目录不存在。
            MFSFileNotFoundException 要移动的源文件或目标文件不存在。

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


    静态成员函数：

    static MFSDataSpace * MFSDataSpace::GetActiveDataSpace() noexcept
        获取应用程序中当前活动的数据空间。若当前应用程序中没有活动的数据空间，返回 nullptr。

    static void MFSDataSpace::SetActiveDataSpace(MFSDataSpace * dataSpace) noexcept
        设置应用程序中的活动的数据空间。此操作不转让传入的指针的所有权。

*/

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

    MFSString GetWorkingDirectory() const noexcept;
    void SetWorkingDirectory(const MFSString & path) noexcept;

    bool Exist(const MFSString & path);
    MFSFile * OpenFile(const MFSString & path, bool createIfNotExist);
    MFSFile * CreateFile(const MFSString & path, bool openIfExist);
    void CreateDirectory(const MFSString & path);
    void CreateLink(const MFSString & target, const MFSString & link);
    void Delete(const MFSString & path);

    void Copy(const MFSString & source, const MFSString & destination);
    void Move(const MFSString & source, const MFSString & destination);

    std::vector<MFSString> GetDirectories(const MFSString & directory);
    std::vector<MFSString> GetFiles(const MFSString & directory);
    
    static MFSDataSpace * GetActiveDataSpace() noexcept;
    static void SetActiveDataSpace(MFSDataSpace * dataSpace) noexcept;

private:
    MFSString _workingDirectory;
    // NOTICE: MFSPartition is an incomplete type and yet we should use std::shared_ptr as below shows.
    std::shared_ptr<MFSPartition> _partition;
};
