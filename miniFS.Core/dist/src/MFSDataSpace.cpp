#include "../include/MFSConfig.h"

#include "../include/MFSDataSpace.h"
#include "../include/MFSPath.h"
#include "../include/exceptions/MFSInvalidArgumentException.h"
#include "../include/exceptions/MFSInvalidPathException.h"
#include "../include/exceptions/MFSInvalidDeviceException.h"
#include "../include/exceptions/MFSInvalidEntryTypeException.h"
#include "../include/exceptions/MFSInvalidOperationException.h"
#include "../include/exceptions/MFSFileAlreadyExistException.h"
#include "../include/exceptions/MFSDirectoryNotFoundException.h"
#include "../include/exceptions/MFSDirectoryAlreadyExistException.h"
#include "../include/exceptions/MFSFileNotFoundException.h"
#include "../include/exceptions/MFSWindowsException.h"
#include "../include/exceptions/MFSOutOfSpaceException.h"
#include "../../include/device/MFSOSFileDevice.h"
#include "../../include/device/MFSOSSparseFileDevice.h"
#include "../../include/device/MFSBlockDevice.h"
#include "../../include/device/MFSSparseBlockDevice.h"
#include "../../include/fs/MFSPartition.h"

#define MFS_DATASPACE_MIN_SIZE      uint64_t(128ull * 1024 * 1024)
#define MFS_DATASPACE_MAX_SIZE      uint64_t(4ull * 1024 * 1024 * 1024)
#define MFS_DEVICE_BLOCK_SIZE       uint64_t(4ull * 1024)


MFSDataSpace::MFSDataSpace(const MFSString & osFileName)
{
    _hFile = CreateFileW(
        osFileName.GetRawString(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (_hFile == INVALID_HANDLE_VALUE)
        throw MFSWindowsException();

#ifdef MFS_ENABLE_SPARSE_FILE
    _fileDevice.reset(new MFSOSSparseFileDevice(_hFile, false));
    _blockDevice.reset(new MFSSparseBlockDevice(dynamic_cast<MFSSparseDevice *>(_fileDevice.get())));
#else
    _fileDevice.reset(new MFSOSFileDevice(_hFile, false));
    _blockDevice.reset(new MFSBlockDevice(_fileDevice.get()));
#endif

    _partition.reset(new MFSPartition(_blockDevice.get()));
    if (!_partition->IsValidDevice())
    {
        Close();
        throw MFSInvalidDeviceException();
    }

	if (!_partition->IsRaw())
		SetWorkingDirectory(L"/");
}

MFSDataSpace::MFSDataSpace(MFSDataSpace && another)
    : _workingDirectory(std::move(another._workingDirectory)),
      _hFile(another._hFile), 
      _fileDevice(std::move(another._fileDevice)),
      _blockDevice(std::move(another._blockDevice)),
      _partition(std::move(another._partition))
{
    another._hFile = INVALID_HANDLE_VALUE;
}

MFSDataSpace & MFSDataSpace::operator=(MFSDataSpace && another)
{
    this->_workingDirectory = std::move(another._workingDirectory);
    this->_hFile = another._hFile;
    this->_fileDevice = std::move(another._fileDevice);
    this->_blockDevice = std::move(another._blockDevice);
    this->_partition = std::move(another._partition);
    another._hFile = INVALID_HANDLE_VALUE;
	return *this;
}

MFSString MFSDataSpace::GetWorkingDirectory() const noexcept
{
    return _workingDirectory;
}

void MFSDataSpace::SetWorkingDirectory(const MFSString & path)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);
    if (MFSPath::IsOSPath(path))
        throw MFSInvalidPathException(path);
    if (!Exist(path))
        throw MFSInvalidPathException(path);

    _workingDirectory = MFSPath::Combine(_workingDirectory, path);
}

uint64_t MFSDataSpace::GetTotalSpaceInBytes() const noexcept
{
    return _partition->GetTotalSpaceInBytes();
}

uint64_t MFSDataSpace::GetFreeSpaceInBytes() const noexcept
{
    return _partition->GetFreeSpaceInBytes();
}

bool MFSDataSpace::IsFormatted() const noexcept
{
    return !_partition->IsRaw();
}

void MFSDataSpace::Format() noexcept
{
    _partition->BuildFileSystem();
	SetWorkingDirectory(L"/");
}

void MFSDataSpace::Optimize() noexcept
{
    std::unique_ptr<MFSPartitionOptimizer> optimizer(_partition->GetOptimizer());
    optimizer->Optimize();
}

bool MFSDataSpace::Exist(const MFSString & path)
{
	MFSFSEntry * entry = nullptr;
	try
	{
		entry = OpenFSEntry(path);
	}
	catch (const MFSDirectoryNotFoundException &)
	{
		return false;
	}
	catch (const MFSFileNotFoundException &)
	{
		return false;
	}

	return true;
}

MFSFile * MFSDataSpace::OpenFile(const MFSString & path, bool createIfNotExist)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString directory = MFSPath::GetDirectoryPath(path);
    std::unique_ptr<MFSFSEntry> directoryFsEntry(OpenFSEntry(directory));
    if (!directoryFsEntry)
        throw MFSDirectoryNotFoundException(directory);

    MFSString filename = MFSPath::GetFileName(path);
    if (filename.IsEmpty())
        throw MFSInvalidPathException(path);

    if (!directoryFsEntry->ContainsSubEntry(filename))
    {
        if (!createIfNotExist)
            throw MFSFileNotFoundException(path);
        else
        {
            MFSFSEntry * fileEntry = directoryFsEntry->AddSubEntry(filename);
            if (!fileEntry)
                throw MFSOutOfSpaceException();

            return new MFSFile(fileEntry);
        }
    }
    else
    {
        // directoryFsEntry->ContainsSubEntry(filename).
        MFSFSEntry * fileEntry = directoryFsEntry->GetSubEntry(filename);
        if (!fileEntry)
            throw MFSException(L"Unexpected null fileEntry.");

        if (fileEntry->GetEntryType() != MFSFSEntryType::File)
            throw MFSDirectoryAlreadyExistException(path);

        return new MFSFile(fileEntry);
    }
}

MFSFile * MFSDataSpace::CreateFile(const MFSString & path, bool openIfExist)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString directory = MFSPath::Combine(GetWorkingDirectory(), MFSPath::GetDirectoryPath(path));
    MFSString filename = MFSPath::GetFileName(path);
    if (filename.IsEmpty())
        throw MFSInvalidPathException(path);
    
    std::unique_ptr<MFSFSEntry> directoryFsEntry(OpenFSEntry(directory));
    if (!directoryFsEntry)
        throw MFSDirectoryNotFoundException(directory);

    if (directoryFsEntry->ContainsSubEntry(filename))
    {
        if (!openIfExist)
            throw MFSFileAlreadyExistException(path);
        else
        {
            MFSFSEntry * fileFsEntry = directoryFsEntry->GetSubEntry(filename);
            if (!fileFsEntry)
                throw MFSException(L"Unexpected null fileFsEntry.");

            if (fileFsEntry->GetEntryType() != MFSFSEntryType::File)
                throw MFSDirectoryAlreadyExistException(path);

            return new MFSFile(directoryFsEntry.get());
        }
    }
    else
    {
        // !directoryFsEntry->ContainsSubEntry(filename).
        MFSFSEntry * fileFsEntry = directoryFsEntry->AddSubEntry(filename);
        if (!fileFsEntry)
            throw MFSOutOfSpaceException();

        fileFsEntry->SetEntryType(MFSFSEntryType::File);
        return new MFSFile(fileFsEntry);
    }
}

void MFSDataSpace::CreateDirectory(const MFSString & path, bool errorIfExist)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString absolute = MFSPath::Combine(GetWorkingDirectory(), path);
    
    std::vector<MFSString> pathNames = MFSPath::GetPathNames(absolute);
    std::unique_ptr<MFSFSEntry> entry(OpenRootFSEntry());

    for (uint32_t i = 0; i < pathNames.size(); ++i)
    {
        const MFSString & name = pathNames[i];
        if (entry->ContainsSubEntry(name))
        {
            MFSFSEntry * subEntry = entry->GetSubEntry(name);
            if (!subEntry)
                throw MFSException(L"Unexpected null subEntry.");
            if (subEntry->GetEntryType() == MFSFSEntryType::File)
                throw MFSFileAlreadyExistException(name);

            entry.reset(subEntry);
        }
        else
        {
            MFSFSEntry * subEntry = entry->AddSubEntry(name);
            if (!subEntry)
                throw MFSOutOfSpaceException();

            subEntry->SetEntryType(MFSFSEntryType::Directory);
            entry.reset(subEntry);
        }
    }
}

void MFSDataSpace::CreateLink(const MFSString & src, const MFSString & target)
{
    std::unique_ptr<MFSFSEntry> targetEntry(OpenFSEntry(src));
    if (!targetEntry)
        throw MFSException(L"Unexpected null targetEntry.");

    uint32_t targetFsnodeId = targetEntry->GetFSNodeId();
    targetEntry.release();

    MFSString linkDirectory = MFSPath::GetDirectoryPath(target);
    MFSString linkFilename = MFSPath::GetFileName(target);
    if (linkFilename.IsEmpty())
        throw MFSInvalidPathException(src);

    std::unique_ptr<MFSFSEntry> linkDirectoryEntry(OpenFSEntry(linkDirectory));
    if (!linkDirectoryEntry)
        throw MFSDirectoryNotFoundException(linkDirectory);

    if (linkDirectoryEntry->ContainsSubEntry(linkFilename))
        throw MFSFileAlreadyExistException(target);

    std::unique_ptr<MFSFSEntry> linkEntry(linkDirectoryEntry->AddSubEntry(linkFilename, targetFsnodeId));
    if (!linkEntry)
        throw MFSOutOfSpaceException();
}

void MFSDataSpace::Delete(const MFSString & path)
{
    MFSString normPath = MFSPath::GetNormalizedPath(path);

    MFSString directory = MFSPath::GetDirectoryPath(normPath);
    MFSString filename = MFSPath::GetFileName(normPath);

    std::unique_ptr<MFSFSEntry> directoryEntry(OpenFSEntry(directory));

    if (!directoryEntry)
        throw MFSDirectoryNotFoundException(directory);
    if (!directoryEntry->ContainsSubEntry(filename))
        throw MFSFileNotFoundException(normPath);

    std::unique_ptr<MFSFSEntry> targetEntry(directoryEntry->GetSubEntry(filename));
    if (!targetEntry)
        throw MFSException(L"Unexpected null targetEntry.");

    if (targetEntry->GetEntryType() == MFSFSEntryType::Directory && targetEntry->GetSubEntriesCount())
        throw MFSInvalidOperationException(L"Deleting a non-empty directory is prohibited.");

    if (!directoryEntry->RemoveSubEntry(filename))
        throw MFSException(L"Unexpected RemoveSubEntry call failed.");
}

MFSEntryInfo MFSDataSpace::GetEntryInfo(const MFSString & path)
{
    MFSString normPath = MFSPath::GetNormalizedPath(path);

    std::unique_ptr<MFSFSEntry> entry(OpenFSEntry(normPath));
    if (!entry)
        throw MFSFileNotFoundException(normPath);

    MFSEntryInfo info;
    info.CreationTime = entry->GetCreationTime();
    info.LastAccessTime = entry->GetLastAccessTime();
    info.LastModificationTime = entry->GetLastModificationTime();
    info.IsDirectory = (entry->GetEntryType() == MFSFSEntryType::Directory);
    info.IsHidden = entry->GetHiddenFlag();

    return info;
}

void MFSDataSpace::SetHidden(const MFSString & path, bool isHidden)
{
    MFSString normPath = MFSPath::GetNormalizedPath(path);

    std::unique_ptr<MFSFSEntry> entry(OpenFSEntry(normPath));
    if (!entry)
        throw MFSFileNotFoundException(normPath);

    entry->SetHiddenFlag(isHidden);
}

void MFSDataSpace::Copy(const MFSString & source, const MFSString & destination)
{
    MFSString normSource = MFSPath::GetNormalizedPath(source);
    MFSString normDest = MFSPath::GetNormalizedPath(destination);

    std::unique_ptr<MFSFSEntry> sourceEntry(OpenFSEntry(normSource));
    if (!sourceEntry)
        throw MFSFileNotFoundException(normSource);

    MFSString destDirectory = MFSPath::GetDirectoryPath(normDest);
    MFSString destFilename = MFSPath::GetFileName(normDest);

    std::unique_ptr<MFSFSEntry> destDirectoryEntry(OpenFSEntry(destDirectory));
    if (!destDirectoryEntry)
        throw MFSDirectoryNotFoundException(destDirectory);

    if (destDirectoryEntry->ContainsSubEntry(destFilename))
        throw MFSFileAlreadyExistException(normDest);

    std::unique_ptr<MFSFSEntry> destEntry(destDirectoryEntry->AddSubEntry(destFilename));
    if (!destEntry)
        throw MFSOutOfSpaceException();

    destEntry->SetEntryType(sourceEntry->GetEntryType());
    
    if (sourceEntry->GetEntryType() == MFSFSEntryType::Directory)
        throw MFSInvalidOperationException(L"Copying a directory is not supported.");
    else
    {
        if (!destEntry->SetFileSize(sourceEntry->GetFileSize()))
            throw MFSOutOfSpaceException();

        std::unique_ptr<MFSStream> sourceStream(sourceEntry->OpenDataStream());
        if (!sourceStream)
            throw MFSException(L"Failed to open data stream of source file.");

        std::unique_ptr<MFSStream> destStream(destEntry->OpenDataStream());
        if (!destStream)
            throw MFSException(L"Failed to open data stream of destination file.");

        std::unique_ptr<uint8_t[]> buffer(new uint8_t[MFS_DEVICE_BLOCK_SIZE]);
        while (sourceStream->HasNext())
        {
            uint32_t read = sourceStream->Read(buffer.get(), MFS_DEVICE_BLOCK_SIZE, MFS_DEVICE_BLOCK_SIZE);
            uint32_t write = destStream->Write(buffer.get(), read);

            if (write != read)
                throw MFSException(L"Failed to write to destination data stream.");
        }

        destStream->Flush();
        destStream->Close();
        sourceStream->Close();
    }
}

void MFSDataSpace::Move(const MFSString & source, const MFSString & destination)
{
    MFSString normSource = MFSPath::GetNormalizedPath(source);
    MFSString normDest = MFSPath::GetNormalizedPath(destination);

	MFSString srcDirectory = MFSPath::GetDirectoryPath(normSource);
	MFSString srcFilename = MFSPath::GetFileName(normSource);
	MFSString dstDirectory = MFSPath::GetDirectoryPath(normDest);
	MFSString dstFilename = MFSPath::GetFileName(normDest);

	std::unique_ptr<MFSFSEntry> srcDirEntry(OpenFSEntry(srcDirectory));
	if (!srcDirEntry)
		throw MFSDirectoryNotFoundException(srcDirectory);
	std::unique_ptr<MFSFSEntry> dstDirEntry(OpenFSEntry(dstDirectory));
	if (!dstDirEntry)
		throw MFSDirectoryNotFoundException(dstDirectory);

	std::unique_ptr<MFSFSEntry> srcFileEntry(srcDirEntry->GetSubEntry(srcFilename));
	if (!srcFileEntry)
		throw MFSFileNotFoundException(normSource);
	std::unique_ptr<MFSFSEntry> dstFileEntry(dstDirEntry->GetSubEntry(dstFilename));
	if (dstFileEntry)
		throw MFSFileAlreadyExistException(normDest);

	if (dstDirEntry->ContainsSubEntry(dstFilename))
		throw MFSFileAlreadyExistException(normDest);

	MFSFSEntry* pEntry = dstDirEntry->AddSubEntry(dstFilename, srcFileEntry->GetFSNodeId());
	if (pEntry)
		delete pEntry;
	else
		throw MFSOutOfSpaceException();

	srcFileEntry.release();
	bool flag = srcDirEntry->RemoveSubEntry(srcFilename);
	if (!flag)
		throw MFSException(L"unexpected RemoveSubEntry failed.");
}

std::vector<MFSString> MFSDataSpace::GetDirectories(const MFSString & directory)
{
    return GetSubEntryNames(directory, MFSFSEntryType::Directory);
}

std::vector<MFSString> MFSDataSpace::GetFiles(const MFSString & directory)
{
    return GetSubEntryNames(directory, MFSFSEntryType::File);
}

void MFSDataSpace::Close() noexcept
{
    if (_partition)
        _partition->Close();
    if (_blockDevice)
        _blockDevice->Close();
    if (_fileDevice)
        _fileDevice->Close();

    if (_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(_hFile);
        _hFile = INVALID_HANDLE_VALUE;
    }
}

MFSDataSpace * MFSDataSpace::CreateDataSpace(const MFSString & filename, uint64_t size)
{
    if (size % MFS_DEVICE_BLOCK_SIZE)
        size = (size / MFS_DEVICE_BLOCK_SIZE + 1) * MFS_DEVICE_BLOCK_SIZE;

    if (size < MFS_DATASPACE_MIN_SIZE || size > MFS_DATASPACE_MAX_SIZE)
        throw MFSInvalidArgumentException(L"The size of the data space to be created is out of range.");

    HANDLE hFile = CreateFileW(
        filename.GetRawString(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE)
        throw MFSWindowsException();

    LARGE_INTEGER liOffset;
    liOffset.QuadPart = size;

    if (!SetFilePointerEx(hFile, liOffset, NULL, FILE_BEGIN)) 
    {
        uint32_t errCode = GetLastError();

        CloseHandle(hFile);
        DeleteFileW(filename.GetRawString());

        throw MFSWindowsException(errCode);
    }
    if (!SetEndOfFile(hFile))
    {
        uint32_t errCode = GetLastError();

        CloseHandle(hFile);
        DeleteFileW(filename.GetRawString());

        throw MFSWindowsException(errCode);
    }

    CloseHandle(hFile);
    return new MFSDataSpace(filename);
}


MFSDataSpace * volatile globalActiveSpace = nullptr;
// CRITICAL_SECTION globalActiveSpaceGuard;

MFSDataSpace * MFSDataSpace::GetActiveDataSpace() noexcept
{
    return globalActiveSpace;
}

void MFSDataSpace::SetActiveDataSpace(MFSDataSpace * dataSpace) noexcept
{
    globalActiveSpace = dataSpace;
}

MFSFSEntry * MFSDataSpace::OpenRootFSEntry()
{
    return _partition->GetRoot();
}

MFSFSEntry * MFSDataSpace::OpenFSEntry(const MFSString & path)
{
    if (path.IsEmpty())
        return OpenFSEntry(_workingDirectory);

    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString absolute = MFSPath::Combine(GetWorkingDirectory(), path);
    
    std::vector<MFSString> pathNames = MFSPath::GetPathNames(absolute);
    MFSFSEntry * entry = OpenRootFSEntry();
    
    for (const MFSString & name : pathNames)
    {
        MFSFSEntry * subEntry = entry->GetSubEntry(name);
        if (!subEntry)
            throw MFSDirectoryNotFoundException(name);
        else
        {
            delete entry;
            entry = subEntry;
        }
    }

    if (MFSPath::IsDirectoryPath(path) && entry->GetEntryType() != MFSFSEntryType::Directory)
    {
        delete entry;
        throw MFSDirectoryNotFoundException(path);
    }

    return entry;
}

std::vector<MFSString> MFSDataSpace::GetSubEntryNames(const MFSString & directory, MFSFSEntryType entryType)
{
    if (!MFSPath::IsValidPath(directory))
        throw MFSInvalidPathException(directory);

    MFSFSEntry * directoryEntry = OpenFSEntry(directory);
    if (!directoryEntry || directoryEntry->GetEntryType() != MFSFSEntryType::Directory)
        throw MFSDirectoryNotFoundException(directory);

    auto subEntries = directoryEntry->GetSubEntries();
    std::vector<MFSString> result;
    for (auto & entryPair : subEntries)
    {
        if (entryPair.second->GetEntryType() == entryType)
        {
            result.push_back(std::move(entryPair.first));
        }

        delete entryPair.second;
    }

    return result;
}
