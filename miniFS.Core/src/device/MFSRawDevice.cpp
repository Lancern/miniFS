#include "../../include/device/MFSRawDevice.h"



MFSRawDevice::MFSRawDevice()
{
}


MFSRawDevice::~MFSRawDevice()
{
}


MFSRawDeviceView * MFSRawDevice::OpenView(UINT64 offset, DWORD viewSize)
{
    return OpenView(offset, viewSize, !CanWrite());
}
