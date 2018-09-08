#include "../include/MFSPath.h"

bool MFSPath::IsValidPath(const MFSString & path) noexcept
{
	// TODO: Implement MFSPath::IsValidPath(const MFSString & path)
	return true;
}

bool MFSPath::IsAbsolutePath(const MFSString & path) noexcept
{
	// TODO: Implement MFSPath::IsAbsolutePath(const MFSString & path)
	return true;
}

bool MFSPath::IsOSPath(const MFSString & path) noexcept
{
	// TODO: Implement MFSPath::IsOSPath(const MFSString & path)
	return true;
}

std::vector<MFSString> MFSPath::GetPathNames(const MFSString & path)
{
	// TODO: Implement std::vector<MFSString> MFSPath::GetPathNames(const MFSString & path)
	std::vector<MFSString> string;
	return string;
}

MFSString MFSPath::GetFileName(const MFSString & path)
{
	// TODO: Implement MFSString MFSPath::GetFileName(const MFSString & path)
	return MFSString(L"");
}

MFSString MFSPath::GetExtension(const MFSString & path)
{
	// TODO: Implement MFSString MFSPath::GetExtension(const MFSString & path)
	return MFSString(L"");
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
	// TODO: Implement wchar_t MFSPath::GetPathSeparator() noexcept
	return L'';
}

std::vector<wchar_t> MFSPath::GetInvalidChars() noexcept
{
	// TODO: Implement std::vector<wchar_t> MFSPath::GetInvalidChars() noexcept
	return L'';
}