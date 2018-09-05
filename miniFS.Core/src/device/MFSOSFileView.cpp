#include "../../include/device/MFSOSFileView.h"



MFSOSFileView::MFSOSFileView(HANDLE hFileMapping, uint64_t offset, uint32_t size, bool readonly)
    : _viewSize(size), _canWrite(!readonly) 
{
    uint32_t access = readonly ? FILE_MAP_READ : FILE_MAP_WRITE;
    _lpFileMappingAddress = MapViewOfFile(hFileMapping, access, offset >> 32, offset & 0xFFFFFFFF, size);
}

MFSOSFileView::~MFSOSFileView() 
{
    Close();
    MFSRawDeviceView::~MFSRawDeviceView();
}


bool MFSOSFileView::CanWrite() const { return _canWrite; }
uint32_t MFSOSFileView::GetSize() const { return _viewSize; }

uint32_t MFSOSFileView::Read(void * lpBuffer, uint32_t offset, uint32_t length) 
{
    if (!lpBuffer)
        return 0;
    if (length == 0)
        return 0;

    memcpy(lpBuffer, reinterpret_cast<const char *>(_lpFileMappingAddress) + offset, length);
    return length;
}

uint32_t MFSOSFileView::Write(uint32_t offset, uint32_t length, const void * lpBuffer) 
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
    if (_canWrite && _lpFileMappingAddress)
        FlushViewOfFile(_lpFileMappingAddress, _viewSize);
}

void MFSOSFileView::Close()
{
    if (_lpFileMappingAddress)
    {
        Flush();
        UnmapViewOfFile(_lpFileMappingAddress);

        _lpFileMappingAddress = NULL;
    }
}
