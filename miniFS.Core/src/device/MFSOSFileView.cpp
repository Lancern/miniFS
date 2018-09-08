#include "../../include/device/MFSOSFileView.h"



MFSOSFileView::MFSOSFileView(HANDLE hFileMapping, uint64_t offset, uint32_t size, bool readonly)
    : _viewSize(size), _canWrite(!readonly) 
{
    uint32_t access = readonly ? FILE_MAP_READ : FILE_MAP_WRITE;

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    uint64_t mappingOffset = offset / sysInfo.dwAllocationGranularity * sysInfo.dwAllocationGranularity;

    _lpFileMappingBaseAddr = MapViewOfFile(hFileMapping, 
        access, 
        mappingOffset >> 32, 
        mappingOffset & 0xFFFFFFFF, 
        size + static_cast<uint32_t>(offset - mappingOffset));
    _lpFileMappingAddress = reinterpret_cast<uint8_t *>(_lpFileMappingBaseAddr) 
        + (offset - mappingOffset);
}

MFSOSFileView::~MFSOSFileView() 
{
    Close();
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
    if (_canWrite && _lpFileMappingBaseAddr)
        FlushViewOfFile(_lpFileMappingBaseAddr, _viewSize);
}

void MFSOSFileView::Close()
{
    if (_lpFileMappingBaseAddr)
    {
        Flush();
        UnmapViewOfFile(_lpFileMappingBaseAddr);

        _lpFileMappingBaseAddr = NULL;
        _lpFileMappingAddress = NULL;
    }
}
