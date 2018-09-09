#include "../include/MFSFile.h"

MFSDateTime MFSFile::GetCreationTime() const noexcept
{
	// TODO: Implement MFSDateTime MFSFile::GetCreationTime() const noexcept
	return MFSDateTime();
}

MFSDateTime MFSFile::GetLastAccessTime() const noexcept
{
	// TODO: Implement MFSDateTime MFSFile::GetLastAccessTime() const noexcept
	return MFSDateTime();
}

MFSDateTime MFSFile::GetLastModificationTime() const noexcept
{
	// TODO: Implement MFSDateTime MFSFile::GetLastAccessTime() const noexcept
	return MFSDateTime();
}

bool MFSFile::IsHidden() const noexcept
{
	// TODO: Implement bool MFSFile::IsHidden() const noexcept
	return true;
}
void MFSFile::SetHidden(bool isHidden) noexcept
{
	// TODO: Implementvoid MFSFile::SetHidden(bool isHidden) noexcept
	return;
}

uint64_t MFSFile::GetFileSize() const noexcept
{
	// TODO: Implementvoid uint64_t MFSFile::GetFileSize() const noexcept
	return 1;
}
void MFSFile::SetFileSize(uint64_t size)
{
	// TODO: Implementvoid void MFSFile::SetFileSize(uint64_t size)
	return;
}

MFSStream * MFSFile::OpenStream() noexcept
{
	// TODO: Implementvoid MFSStream MFSFile::* OpenStream() noexcept
	return nullptr;
}

MFSFile::~MFSFile()
{

}

MFSFile::MFSFile(MFSFSEntry * entry)
{

}