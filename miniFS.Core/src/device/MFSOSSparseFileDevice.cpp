#include "../../include/device/MFSOSSparseFileDevice.h"
#include <memory>


MFSOSSparseFileDevice::MFSOSSparseFileDevice(HANDLE hFile, bool readonly)
    : MFSOSFileDevice(hFile, readonly)
{
    // Obtain the actual allocated size of the file on the disk.
    std::unique_ptr<wchar_t[]> filename(new wchar_t[128]);
    DWORD acq = GetFinalPathNameByHandleW(hFile, filename.get(), 127, 0);
    if (acq >= 128)
    {
        filename.reset(new wchar_t[acq]);
        GetFinalPathNameByHandleW(hFile, filename.get(), acq - 1, 0);
    }

    DWORD sizeLow, sizeHigh;
    sizeLow = GetCompressedFileSizeW(filename.get(), &sizeHigh);

    _actualSize = (static_cast<uint64_t>(sizeHigh) << 32) | sizeLow;

    DWORD osFsFlags;
    if (!GetVolumeInformationByHandleW(GetFileHandle(), NULL, 0, NULL, 0, &osFsFlags, NULL, 0))
        _supportSparse = false;
    else
        _supportSparse = static_cast<bool>(osFsFlags & FILE_SUPPORTS_SPARSE_FILES);

    if (_supportSparse)
    {
        // Make the file sparse.
        FILE_SET_SPARSE_BUFFER fssb;
        fssb.SetSparse = TRUE;
        if (!DeviceIoControl(GetFileHandle(), FSCTL_SET_SPARSE, &fssb, sizeof(fssb), NULL, 0, NULL, NULL))
            _supportSparse = false;
    }
}

uint64_t MFSOSSparseFileDevice::GetActualAllocatedSize() const
{
    return _actualSize;
}

bool MFSOSSparseFileDevice::ZeroRegion(uint64_t startOffset, uint64_t length)
{
    if (!_supportSparse)
    {
        // TODO: Add code here to collapse a ZeroRegion request into a sequence of normal write.
        return false;
    }
    else
    {
        FILE_ZERO_DATA_INFORMATION info;
        info.FileOffset.QuadPart = startOffset;
        info.BeyondFinalZero.QuadPart = startOffset + length;

        return DeviceIoControl(GetFileHandle(), FSCTL_SET_ZERO_DATA, &info, sizeof(info), NULL, 0, NULL, NULL);
    }
}

bool MFSOSSparseFileDevice::SupportSparse() const
{
    return _supportSparse;
}
