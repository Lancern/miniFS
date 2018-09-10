#include "../include/MFSDataSpace.h"
#include "../include/MFSPath.h"
#include "../include/exceptions/MFSInvalidArgumentException.h"
#include "../include/exceptions/MFSInvalidPathException.h"
#include "../include/exceptions/MFSInvalidDeviceException.h"
#include "../include/exceptions/MFSFileAlreadyExistException.h"
#include "../include/exceptions/MFSDirectoryNotFoundException.h"
#include "../include/exceptions/MFSFileNotFoundException.h"
#include "../include/exceptions/MFSWindowsException.h"
#include "../include/exceptions/MFSOutOfSpaceException.h"
#include "../../include/device/MFSOSFileDevice.h"
#include "../../include/device/MFSBlockDevice.h"
#include "../../include/fs/MFSPartition.h"

#define MFS_DATASPACE_MIN_SIZE      uint64_t(128ull * 1024 * 1024)
#define MFS_DATASPACE_MAX_SIZE      uint64_t(4ull * 1024 * 1024 * 1024)
#define MFS_DEVICE_BLOCK_SIZE       uint64_t(4ull * 1024 * 1024)


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

    _fileDevice.reset(new MFSOSFileDevice(_hFile, false));
    _blockDevice.reset(new MFSBlockDevice(_fileDevice.get()));
    _partition.reset(new MFSPartition(_blockDevice.get()));
    if (!_partition->IsValidDevice())
    {
        Close();
        throw MFSInvalidDeviceException();
    }

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
        throw MFSInvalidPathException(L"The path given is a OS path.");
    if (!Exist(path))
        throw MFSInvalidPathException(L"The path given is not exist.");

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
}

bool MFSDataSpace::Exist(const MFSString & path)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString directory = MFSPath::GetDirectoryPath(path);
    MFSFSEntry * directoryFsEntry = nullptr;

    try
    {
        directoryFsEntry = OpenFSEntry(path);
    }
    catch (const MFSDirectoryNotFoundException &)
    {
        return false;
    }

    if (!directoryFsEntry)
        return false;

    MFSString filename = MFSPath::GetFileName(path);
    bool result = directoryFsEntry->ContainsSubEntry(filename);
    delete directoryFsEntry;

    return result;
}

MFSFile * MFSDataSpace::OpenFile(const MFSString & path, bool createIfNotExist)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString directory = MFSPath::GetDirectoryPath(path);
    MFSFSEntry * directoryFsEntry = OpenFSEntry(directory);
    if (!directoryFsEntry)
        throw MFSDirectoryNotFoundException(directory);

    MFSString filename = MFSPath::GetFileName(path);
    if (!directoryFsEntry->ContainsSubEntry(filename))
    {
        if (!createIfNotExist)
            throw MFSFileNotFoundException(path);
        else
        {
            MFSFSEntry * fileEntry = directoryFsEntry->AddSubEntry(filename);
            delete directoryFsEntry;
            if (!fileEntry)
                throw MFSOutOfSpaceException();

            return new MFSFile(fileEntry);
        }
    }
    else
    {
        // directoryFsEntry->ContainsSubEntry(filename).
        MFSFSEntry * fileEntry = directoryFsEntry->GetSubEntry(filename);
        delete directoryFsEntry;
        if (!fileEntry)
            throw MFSException(L"Unexpected null fileEntry.");

        return new MFSFile(fileEntry);
    }
}

MFSFile * MFSDataSpace::CreateFile(const MFSString & path, bool openIfExist)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString directory = MFSPath::Combine(GetWorkingDirectory(), MFSPath::GetDirectoryPath(path));
    MFSString filename = MFSPath::GetFileName(path);
    
    MFSFSEntry * directoryFsEntry = OpenFSEntry(directory);
    if (!directoryFsEntry)
        throw MFSDirectoryNotFoundException(directory);

    if (directoryFsEntry->ContainsSubEntry(filename))
    {
        if (!openIfExist)
        {
            delete directoryFsEntry;
            throw MFSFileAlreadyExistException(path);
        }
        else
        {
            MFSFSEntry * fileFsEntry = directoryFsEntry->GetSubEntry(filename);
            delete directoryFsEntry;

            if (!fileFsEntry)
                throw MFSException(L"Unexpected null fileFsEntry.");

            return new MFSFile(directoryFsEntry);
        }
    }
    else
    {
        // !directoryFsEntry->ContainsSubEntry(filename).
        MFSFSEntry * fileFsEntry = directoryFsEntry->AddSubEntry(filename);
        delete directoryFsEntry;

        if (!fileFsEntry)
            throw MFSOutOfSpaceException();
        return new MFSFile(fileFsEntry);
    }
}

void MFSDataSpace::CreateDirectory(const MFSString & path)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException(path);

    MFSString absolute = MFSPath::Combine(GetWorkingDirectory(), path);
    
    std::vector<MFSString> pathNames = MFSPath::GetPathNames(absolute);
    MFSFSEntry * entry = OpenRootFSEntry();

    for (const MFSString & name : pathNames)
    {
        if (entry->ContainsSubEntry(name))
        {
            MFSFSEntry * subEntry = entry->GetSubEntry(name);
            delete entry;

            if (!subEntry)
                throw MFSException(L"Unexpected null subEntry.");
            if (subEntry->GetEntryType() == MFSFSEntryType::File)
                throw MFSFileAlreadyExistException(name);

            entry = subEntry;
        }
        else
        {
            MFSFSEntry * subEntry = entry->AddSubEntry(name);
            delete entry;

            if (!subEntry)
                throw MFSOutOfSpaceException();

            subEntry->SetEntryType(MFSFSEntryType::Directory);
            entry = subEntry;
        }
    }

    delete entry;
}

void MFSDataSpace::CreateLink(const MFSString & target, const MFSString & link)
{
    MFSFSEntry * targetEntry = OpenFSEntry(target);
    if (!targetEntry)
        throw MFSException(L"Unexpected null targetEntry.");

    uint32_t targetFsnodeId = targetEntry->GetFSNodeId();
    delete targetEntry;

    MFSString linkDirectory = MFSPath::GetDirectoryPath(link);
    MFSString linkFilename = MFSPath::GetFileName(link);

    MFSFSEntry * linkDirectoryEntry = OpenFSEntry(linkDirectory);
    if (!linkDirectoryEntry)
        throw MFSDirectoryNotFoundException(linkDirectory);

    if (linkDirectoryEntry->ContainsSubEntry(linkFilename))
        throw MFSFileAlreadyExistException(link);

    MFSFSEntry * linkEntry = linkDirectoryEntry->AddSubEntry(linkFilename, targetFsnodeId);
    if (!linkEntry)
        throw MFSOutOfSpaceException();

    delete linkEntry;
}

void MFSDataSpace::Delete(const MFSString & path)
{
    MFSString directory = MFSPath::GetDirectoryPath(path);
    MFSString filename = MFSPath::GetFileName(path);

    // UNDONE: Implement MFSDataSpace::Delete(const MFSString &).
}

void MFSDataSpace::Copy(const MFSString & source, const MFSString & destination)
{
    // TODO: Implement MFSDataSpace::Copy(const MFSString & source, const MFSString & destination).
}

void MFSDataSpace::Move(const MFSString & source, const MFSString & destination)
{
	MFSString srcDirectory = MFSPath::GetDirectoryPath(source);
	MFSString srcFilename = MFSPath::GetFileName(source);
	MFSString dstDirectory = MFSPath::GetDirectoryPath(destination);
	MFSString dstFilename = MFSPath::GetFileName(destination);

	std::unique_ptr<MFSFSEntry> srcDirEntry(OpenFSEntry(srcDirectory));
	if (srcDirEntry == nullptr)
		throw MFSDirectoryNotFoundException(srcDirectory);
	std::unique_ptr<MFSFSEntry> dstDirEntry(OpenFSEntry(dstDirectory));
	if (dstDirEntry == nullptr)
		throw MFSDirectoryNotFoundException(dstDirectory);

	std::unique_ptr<MFSFSEntry> srcFileEntry(srcDirEntry->GetSubEntry(srcFilename));
	if (srcFileEntry == nullptr)
		throw MFSFileNotFoundException(source);
	std::unique_ptr<MFSFSEntry> dstFileEntry(dstDirEntry->GetSubEntry(dstFilename));
	if (dstFileEntry == nullptr)
		throw MFSFileAlreadyExistException(destination);

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
        CREATE_ALWAYS,
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
