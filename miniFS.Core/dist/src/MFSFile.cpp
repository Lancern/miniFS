#include "../include/MFSFile.h"
#include "../../include/fs/MFSPartition.h"
#include "../include/exceptions/MFSOutOfSpaceException.h"

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

MFSString MFSFile::GetFileName() const noexcept
{
    return _filename;
}

MFSFile::MFSFile(MFSFSEntry * entry, const MFSString & filename)
	: _entry(entry), _filename(filename)
{
}