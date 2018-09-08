#include "../../include/device/MFSRawDevice.h"



MFSRawDevice::MFSRawDevice()
{
}


MFSRawDevice::~MFSRawDevice()
{
}


MFSRawDeviceView * MFSRawDevice::OpenView(uint64_t offset, uint32_t viewSize)
{
    return OpenView(offset, viewSize, !CanWrite());
}
