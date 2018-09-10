#include "../include/MFSFile.h"
#include "../../include/fs/MFSPartition.h"
#include "../include/exceptions/MFSOutOfSpaceException.h"

MFSDateTime MFSFile::GetCreationTime() const noexcept
{
	return _entry->GetCreationTime();
}

MFSDateTime MFSFile::GetLastAccessTime() const noexcept
{
	return _entry->GetLastAccessTime();
}

MFSDateTime MFSFile::GetLastModificationTime() const noexcept
{
	return _entry->GetLastAccessTime();
}

bool MFSFile::IsHidden() const noexcept
{
	return _entry->GetHiddenFlag();
}
void MFSFile::SetHidden(bool isHidden) noexcept
{
	_entry->SetHiddenFlag(isHidden);
}

uint64_t MFSFile::GetFileSize() const noexcept
{
	return _entry->GetFileSize();
}
void MFSFile::SetFileSize(uint64_t size)
{
	bool flag = _entry->SetFileSize(size);
	if (!flag)
		throw MFSOutOfSpaceException();
}

MFSStream * MFSFile::OpenStream() noexcept
{
	return _entry->OpenDataStream();
}

std::vector<uint32_t> MFSFile::GetAllBlocksId() const noexcept
{
	return _entry->GetAllBlocksId();
}

MFSFile::~MFSFile()
{
}

MFSFile::MFSFile(MFSFSEntry * entry)
	: _entry(entry)
{
}