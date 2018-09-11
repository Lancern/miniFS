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

private:
    MFSSparseDevice * _sparseDevice;
};
