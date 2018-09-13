#pragma once

#include "MFSBlockDevice.h"
#include "MFSSparseDevice.h"


class MFSSparseBlockDevice
    : public MFSBlockDevice
{
public:
    MFSSparseBlockDevice(MFSSparseDevice * device);

    MFSSparseDevice * GetSparseDevice() const;

    bool ZeroBlock(uint64_t blockId);
    bool ZeroBlocks(uint64_t startBlock, uint64_t blockCount);

private:
    MFSSparseDevice * _sparseDevice;
};
