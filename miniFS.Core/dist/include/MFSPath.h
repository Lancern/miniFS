#pragma once

#include "../../include/MFSString.h"
#include <vector>



/*

class MFSPath
提供对文件系统路径的统一操作。

    静态成员函数：

    static bool MFSPath::IsValidPath(const MFSString & path) noexcept
        检查给定的路径是否为一个合法的 mini-FS 路径。
        若给定的路径是一个 OS 路径，此函数将直接返回 false。

    static bool MFSPath::IsAbsolutePath(const MFSString & path) noexcept
        检查给定的路径是否为一个绝对路径。

    static bool MFSPath::IsOSPath(const MFSString & path) noexcept
        检查给定的路径是否为底层操作系统的文件路径。

    static std::vector<MFSString> MFSPath::GetNames(const MFSString & path)
        获取给定的路径中的各个文件或目录名称。
        @exceptions
            MFSInvalidPathException：给定的路径不是一个合法的路径。

    static MFSString MFSPath::GetFileName(const MFSString & path)
        获取给定的路径所表示的文件名。
        @exceptions
            MFSInvalidPathException：给定的路径不是一个合法路径。

    static MFSString MFSPath::GetExtension(const MFSString & path)
        获取给定的路径所表示的文件的拓展名。若给定路径表示的文件存在拓展名，则返回的拓展名中包含前导 dot 字符（.）；
        否则返回一个空串。
        @exceptions
            MFSInvalidPathException：给定的路径不是一个合法路径。

    static MFSString MFSPath::GetFileNameWithoutExtension(const MFSString & path)
        获取给定的路径所表示的文件的不包含拓展名的文件名。
        @exceptions
            MFSInvalidPathException：给定的路径不是一个合法路径。

    static MFSString MFSPath::GetDirectoryPath(const MFSString & path)
        获取给定的路径所表示的文件或目录的父目录。
        @exceptions
            MFSInvalidPathException：给定的路径不是一个合法路径。

    static MFSString MFSPath::Combine(const MFSString & path1, const MFSString & path2)
        获取 path2 在以 path1 为相对参考意义下的绝对路径。
        @exceptions
            MFSInvalidPathException：path1 或 path2 不是一个合法路径。

    static MFSString MFSPath::GetAbsolutePath(const MFSString & path)
        获取给定的路径的绝对路径形式。
        @exceptions
            MFSInvalidPathException：给定的路径不是一个合法路径。
            MFSDataSpaceNotLoadedException：当前没有任何已挂载的数据空间。

    static wchar_t MFSPath::GetPathSeparator() noexcept
        获取系统中用于分隔路径中文件或目录名的字符。

    static std::vector<wchar_t> MFSPath::GetInvalidNameChars() noexcept
        获取系统中定义的无效文件或目录名字符。

*/


class MFSPath
{
public:
    static bool IsValidPath(const MFSString & path) noexcept;
    static bool IsAbsolutePath(const MFSString & path) noexcept;
    static bool IsOSPath(const MFSString & path) noexcept;

    static std::vector<MFSString> GetPathNames(const MFSString & path);

    static MFSString GetFileName(const MFSString & path);
    static MFSString GetExtension(const MFSString & path);
    static MFSString GetFileNameWithoutExtension(const MFSString & path);
    static MFSString GetDirectoryPath(const MFSString & path);

    static MFSString Combine(const MFSString & path1, const MFSString & path2);
    static MFSString GetAbsolutePath(const MFSString & path);

    static wchar_t GetPathSeparator() noexcept;
    static std::vector<wchar_t> GetInvalidNameChars() noexcept;
};
