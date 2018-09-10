#include "../include/MFSPath.h"
#include "../include/MFSDataSpace.h"
#include "../include/exceptions/MFSInvalidPathException.h"
#include "../include/exceptions/MFSDataSpaceNotLoadedException.h"
#include <wctype.h>
#include <algorithm>


bool MFSPath::IsValidPath(const MFSString & path) noexcept
{
    if (IsOSPath(path))
        return false;
	if (path.IsEmpty())
		return true;
	if (path.StartsWith(L" ") || path.EndsWith(L" "))
		return false;

    std::vector<wchar_t> invalidChars = GetInvalidNameChars();

    wchar_t sep = GetPathSeparator();
    for (uint32_t i = 0; i < path.GetLength(); ++i) 
    {
        if (path[i] == sep)
        {
			if (i > 0 && path[i - 1] == L' ')
				return false;
			if (i < path.GetLength() - 1 && path[i + 1] == L' ')
				return false;

            if (i > 0 && path[i - 1] == sep)
            {
                // Consecuive separator is invalid.
                return false;
            }
        }
        else
        {
            if (std::find(invalidChars.begin(), invalidChars.end(), path[i]) != invalidChars.end())
            {
                // Invalid character encountered.
                return false;
            }
        }
    }

    return true;
}

bool MFSPath::IsAbsolutePath(const MFSString & path) noexcept
{
	if (!IsValidPath(path))
		return false;
    return path.StartsWith(L"/");
}

bool MFSPath::IsOSPath(const MFSString & path) noexcept
{
    if (path.GetLength() < 3)
        return false;
    return ((path[0] >= L'a' && path[0] <= L'z') ||
		(path[0] >= L'A' && path[0] <= L'Z')) &&
		path[1] == L':' && path[2] == L'\\';
}

std::vector<MFSString> MFSPath::GetPathNames(const MFSString & path)
{
    if (!IsValidPath(path))
        throw MFSInvalidPathException(path);

    return path.Split({ GetPathSeparator() }, true);
}

MFSString MFSPath::GetFileName(const MFSString & path)
{
    if (!IsValidPath(path))
        throw MFSInvalidPathException(path);
    if (path.GetLength() == 0)
        return path;
	if (path == L"." || path == L"..")
		return MFSString::GetEmptyString();

	if (path.EndsWith(L"/"))
		return MFSString::GetEmptyString();

    wchar_t sep = GetPathSeparator();
    bool endWithSep = path.EndsWith(sep);

    if (endWithSep && path.GetLength() == 1)
        return MFSString::GetEmptyString();

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
        return MFSString::GetEmptyString();
    else
        return path.Substring(nameStart, length);
}

MFSString MFSPath::GetExtension(const MFSString & path)
{
    if (!IsValidPath(path))
        throw MFSInvalidPathException(path);

	if (path == L"." || path == L"..")
		return MFSString::GetEmptyString();

    MFSString name = GetFileName(path);
    if (name.IsEmpty())
        return name;
    
    int startIndex = name.LastIndexOf(L".");
    if (startIndex == -1 || startIndex == name.GetLength() - 1)
        return MFSString::GetEmptyString();
    else
        return name.Substring(startIndex);
}

MFSString MFSPath::GetFileNameWithoutExtension(const MFSString & path)
{
    if (!IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString name = GetFileName(path);
    if (name.IsEmpty())
        return name;

    uint32_t extensionLength = GetExtension(path).GetLength();
    return name.Substring(0, name.GetLength() - extensionLength);
}

MFSString MFSPath::GetDirectoryPath(const MFSString & path)
{
    if (!IsValidPath(path))
        throw MFSInvalidPathException(path);

    wchar_t sep = GetPathSeparator();
    int lastSep = path.LastIndexOf(sep);
    if (lastSep == -1)
        return MFSString::GetEmptyString();
    
    return path.Substring(0, lastSep);
}

MFSString MFSPath::Combine(const MFSString & path1, const MFSString & path2)
{
	if (path1.IsEmpty())
		return path2;
	if (path2.IsEmpty())
		return path1;

    if (!IsValidPath(path1))
		throw MFSInvalidPathException(path1);
	if (!IsValidPath(path2))
        throw MFSInvalidPathException(path2);
    if (IsAbsolutePath(path2))
        return path2;

    bool absolute = IsAbsolutePath(path1);

    std::vector<MFSString> path1Names = GetPathNames(path1);
    std::vector<MFSString> path2Names = GetPathNames(path2);
    
    std::vector<MFSString> resultNames;
    for (auto v : { &path1Names, &path2Names })
    {
        for (const MFSString & name : *v)
        {
            if (name == L".")
                continue;
            else if (name == L"..")
            {
                if (resultNames.empty() || resultNames.back() == L"..")
                {
                    if (!absolute)
                    {
                        resultNames.push_back(name);
                    }
                }
                else
                    resultNames.pop_back();
            }
            else
            {
                resultNames.push_back(name);
            }
        }
    }

    MFSString result = MFSString::Join(GetPathSeparator(), resultNames.begin(), resultNames.end());
    if (absolute)
        return GetPathSeparator() + result;
    else
        return result;
}

MFSString MFSPath::GetAbsolutePath(const MFSString & path)
{
    if (!IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSDataSpace * activeSpace = MFSDataSpace::GetActiveDataSpace();
    if (!activeSpace)
        throw MFSDataSpaceNotLoadedException();

    return Combine(activeSpace->GetWorkingDirectory(), path);
}

wchar_t MFSPath::GetPathSeparator() noexcept
{
    return L'/';
}

std::vector<wchar_t> MFSPath::GetInvalidNameChars() noexcept
{
    return { L'|', L'"', L'\'' };
}
