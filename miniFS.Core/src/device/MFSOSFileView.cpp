#include "../../include/device/MFSOSFileView.h"



MFSOSFileView::MFSOSFileView(HANDLE hFileMapping, UINT64 offset, DWORD size, bool readonly)
    : _viewSize(size), _canWrite(!readonly) 
{
    DWORD access = readonly ? FILE_MAP_READ : FILE_MAP_WRITE;
    _lpFileMappingAddress = MapViewOfFile(hFileMapping, access, offset >> 32, offset & 0xFFFFFFFF, size);
}

MFSOSFileView::~MFSOSFileView() 
{
    Close();
    MFSRawDeviceView::~MFSRawDeviceView();
}


bool MFSOSFileView::CanWrite() const { return _canWrite; }
DWORD MFSOSFileView::GetSize() const { return _viewSize; }

DWORD MFSOSFileView::Read(LPVOID lpBuffer, DWORD offset, DWORD length) 
{
    if (!lpBuffer)
        return 0;
    if (length == 0)
        return 0;

    memcpy(lpBuffer, reinterpret_cast<const char *>(_lpFileMappingAddress) + offset, length);
    return length;
}

DWORD MFSOSFileView::Write(DWORD offset, DWORD length, LPCVOID lpBuffer) 
{
    if (!_canWrite)
        return 0;
    if (length == 0)
        return 0;
    if (!lpBuffer)
        return 0;

    memcpy(reinterpret_cast<char *>(_lpFileMappingAddress) + offset, lpBuffer, length);
    return length;
}

void MFSOSFileView::Flush() 
{
    if (_canWrite)
        FlushViewOfFile(_lpFileMappingAddress, _viewSize);
}

void MFSOSFileView::Close()
{
    Flush();
    UnmapViewOfFile(_lpFileMappingAddress);
}
