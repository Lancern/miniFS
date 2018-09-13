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
    return ZeroBlocks(blockId, 1);
}

bool MFSSparseBlockDevice::ZeroBlocks(uint64_t startBlock, uint64_t blockCount)
{
    uint32_t blockSize = GetBlockSize();
    uint64_t offset = startBlock * blockSize;
    uint64_t length = blockCount * blockSize;
    return _sparseDevice->ZeroRegion(offset, length);
}
