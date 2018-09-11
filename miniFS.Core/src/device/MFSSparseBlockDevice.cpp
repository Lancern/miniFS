#include "../../include/device/MFSSparseBlockDevice.h"


MFSSparseBlockDevice::MFSSparseBlockDevice(MFSSparseDevice * device)
    : MFSBlockDevice(device), _sparseDevice(device)
{
}

MFSSparseDevice * MFSSparseBlockDevice::GetSparseDevice() const
{
    return _sparseDevice;
}

bool MFSSparseBlockDevice::ZeroBlock(uint64_t blockId)
{
    uint32_t blockSize = GetBlockSize();
    uint64_t offset = blockSize * blockId;
    return _sparseDevice->ZeroRegion(offset, blockSize);
}
