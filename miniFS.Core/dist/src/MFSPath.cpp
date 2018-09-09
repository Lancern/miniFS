#include "../include/MFSPath.h"
#include "../include/exceptions/MFSInvalidPathException.h"
#include <wctype.h>


bool MFSPath::IsValidPath(const MFSString & path) noexcept
{
    if (IsOSPath(path))
        return true;

    // TODO: Implement MFSPath::IsValidPath(const MFSString &).
    return true;
}

bool MFSPath::IsAbsolutePath(const MFSString & path) noexcept
{
    return path.StartsWith(L"/");
}

bool MFSPath::IsOSPath(const MFSString & path) noexcept
{
    if (path.GetLength() < 3)
        return false;
    return iswalpha(path[0]) && path[1] == L':' && path[2] == L'\\';
}

std::vector<MFSString> MFSPath::GetPathNames(const MFSString & path)
{
    if (!IsValidPath(path))
        throw MFSInvalidPathException();

    return path.Split({ GetPathSeparator() }, true);
}

MFSString MFSPath::GetFileName(const MFSString & path)
{
    if (!IsValidPath(path))
        throw MFSInvalidPathException();
    if (path.GetLength() == 0)
        return path;

    wchar_t sep = GetPathSeparator();
    bool endWithSep = path.EndsWith(sep);

    if (endWithSep && path.GetLength() == 1)
        return MFSString::GetEmpty();

    int nameStart = endWithSep ? path.GetLength() - 2 : path.GetLength() - 1;
    
    while (nameStart >= 0)
    {
        if (path[nameStart] == sep)
            break;

        --nameStart;
    }

    ++nameStart;
    
    int length = endWithSep
        ? path.GetLength() - 1 - nameStart
        : path.GetLength() - nameStart;

    if (length == 0)
        return MFSString::GetEmpty();
    else
        return path.Substring(nameStart, length);
}

MFSString MFSPath::GetExtension(const MFSString & path)
{
    MFSString name = GetFileName(path);
    if (name.IsEmpty())
        return name;
    
    int startIndex = name.LastIndexOf(L".");
    if (startIndex == -1)
        return MFSString::GetEmpty();
    else
        return name.Substring(startIndex);
}

MFSString MFSPath::GetFileNameWithoutExtension(const MFSString & path)
{
	// TODO: Implement MFSString MFSPath::GetFileNameWithoutExtension(const MFSString & path)
	return MFSString(L"");
}

MFSString MFSPath::GetDirectoryPath(const MFSString & path)
{
	// TODO: Implement MFSString MFSPath::GetDirectoryPath(const MFSString & path)
	return MFSString(L"");
}

MFSString MFSPath::Combine(const MFSString & path1, const MFSString & path2)
{
	// TODO: Implement MFSString MFSPath::Combine(const MFSString & path1, const MFSString & path2)
	return MFSString(L"");
}

MFSString MFSPath::GetAbsolutePath(const MFSString & path)
{
	// TODO: Implement MFSString MFSPath::GetAbsolutePath(const MFSString & path)
	return MFSString(L"");
}

wchar_t MFSPath::GetPathSeparator() noexcept
{
    return L'/';
}

std::vector<wchar_t> MFSPath::GetInvalidNameChars() noexcept
{
	// TODO: Implement std::vector<wchar_t> MFSPath::GetInvalidNameChars() noexcept
	return std::vector<wchar_t>();
}
