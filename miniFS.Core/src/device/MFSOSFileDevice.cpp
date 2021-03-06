#include "../../include/device/MFSOSFileDevice.h"
#include "../../include/device/MFSOSFileView.h"


MFSOSFileDevice::MFSOSFileDevice(HANDLE hFile, bool readonly)
    : _hFile(hFile), _canWrite(!readonly)
{
    _hFileMapping = CreateFileMappingW(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);

    // Get the size of the file.
    DWORD fileSizeLow, fileSizeHigh;
    fileSizeLow = GetFileSize(hFile, &fileSizeHigh);
    _fileSize = (static_cast<uint64_t>(fileSizeHigh) << 32) | fileSizeLow;
}

MFSOSFileDevice::~MFSOSFileDevice()
{
    Close();
    MFSRawDevice::~MFSRawDevice();
}

HANDLE MFSOSFileDevice::GetFileHandle() const
{
    return _hFile;
}


bool MFSOSFileDevice::CanWrite() const { return _canWrite; }
uint64_t MFSOSFileDevice::GetTotalSize() const { return _fileSize; }

MFSRawDeviceView * MFSOSFileDevice::OpenView(uint64_t offset, uint32_t length, bool readonly) 
{
    if (!readonly && !_canWrite)
        return nullptr;

    MFSRawDeviceView * view = new MFSOSFileView(_hFileMapping, offset, length, readonly);
    return view;
}

void MFSOSFileDevice::Close()
{
    if (_hFileMapping)
    {
        CloseHandle(_hFileMapping);
        _hFileMapping = NULL;
    }
}
