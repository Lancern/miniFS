#include "MFSOSFileDevice.h"
#include "MFSOSFileView.h"


MFSOSFileDevice::MFSOSFileDevice(HANDLE hFile, bool readonly)
    : _hFile(hFile), _canWrite(!readonly)
{
    _hFileMapping = CreateFileMappingW(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);

    // Get the size of the file.
    DWORD fileSizeLow, fileSizeHigh;
    fileSizeLow = GetFileSize(hFile, &fileSizeHigh);
    _fileSize = (static_cast<UINT64>(fileSizeHigh) << 32) | fileSizeLow;
}

MFSOSFileDevice::~MFSOSFileDevice()
{
    Close();
    MFSRawDevice::~MFSRawDevice();
}


bool MFSOSFileDevice::CanWrite() const { return _canWrite; }
UINT64 MFSOSFileDevice::GetTotalSize() const { return _fileSize; }

MFSRawDeviceView * MFSOSFileDevice::OpenView(UINT64 offset, DWORD length) 
{
    MFSRawDeviceView * view = new MFSOSFileView(_hFileMapping, offset, length, !_canWrite);
    return view;
}

void MFSOSFileDevice::Close()
{
    CloseHandle(_hFileMapping);
}
