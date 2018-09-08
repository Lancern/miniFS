#include "../include/MFSDataSpace.h"
#include "../include/MFSPath.h"
#include "../include/exceptions/MFSInvalidArgumentException.h"
#include "../include/exceptions/MFSInvalidPathException.h"
#include "../include/exceptions/MFSInvalidDeviceException.h"
#include "../include/exceptions/MFSWindowsException.h"
#include "../../include/device/MFSOSFileDevice.h"
#include "../../include/device/MFSBlockDevice.h"
#include "../../include/fs/MFSPartition.h"

#define MFS_DATASPACE_MIN_SIZE      (128 * 1024 * 1024)
#define MFS_DATASPACE_MAX_SIZE      (4 * 1024 * 1024 * 1024)
#define MFS_DEVICE_BLOCK_SIZE       (4 * 1024 * 1024)


MFSDataSpace::MFSDataSpace(const MFSString & osFileName)
{
    _hFile = CreateFileW(
        osFileName.GetRawString(),
        GENERIC_ALL,
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
}

MFSString MFSDataSpace::GetWorkingDirectory() const noexcept
{
    return _workingDirectory;
}

void MFSDataSpace::SetWorkingDirectory(const MFSString & path)
{
    if (!MFSPath::IsValidPath(path))
        throw MFSInvalidPathException();
    if (!MFSPath::IsOSPath(path))
        throw MFSInvalidPathException(L"The path given is a OS path.");
    if (!Exist(path))
        throw MFSInvalidPathException(L"The path given is not exist.");

    _workingDirectory = path;
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
    // TODO: Implement MFSDataSpace::Exist(const MFSString &).
    return true;
}

MFSFile * MFSDataSpace::OpenFile(const MFSString & path, bool createIfNotExist)
{
    // TODO: Implement MFSDataSpace::OpenFile(const MFSString &, bool).
    return nullptr;
}

MFSFile * MFSDataSpace::CreateFile(const MFSString & path, bool openIfExist)
{
    // TODO: Implement MFSDataSpace::CreateFile(const MFSString &, bool).
    return nullptr;
}

void MFSDataSpace::CreateDirectory(const MFSString & path)
{
    // TODO: Implement MFSDataSpace::CreateDirectory(const MFSString &).
}

void MFSDataSpace::CreateLink(const MFSString & target, const MFSString & link)
{
    // TODO: Implement MFSDataSpace::CreateLink(const MFSString &).
}

void MFSDataSpace::Delete(const MFSString & path)
{
    // TODO: Implement MFSDataSpace::Delete(const MFSString &).
}

void MFSDataSpace::Copy(const MFSString & source, const MFSString & destination)
{
    // TODO: Implement MFSDataSpace::Copy(const MFSString & source, const MFSString & destination).
}

void MFSDataSpace::Move(const MFSString & source, const MFSString & destination)
{
    // TODO: Implement MFSDataSpace::Move(const MFSString &, const MFSString &).
}

std::vector<MFSString> MFSDataSpace::GetDirectories(const MFSString & directory)
{
    // TODO: Implement MFSDataSpace::GetDirectories(const MFSString &).
    return std::vector<MFSString>();
}

std::vector<MFSString> MFSDataSpace::GetFiles(const MFSString & directory)
{
    // TODO: Implement MFSDataSpace::GetFiles(const MFSString &).
    return std::vector<MFSString>();
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
        GENERIC_ALL,
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
