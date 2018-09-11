#pragma once
#pragma warning(disable: 4250)  // Suppress MSVC warning C4250: "Inherits XXX via dominance".

#include "MFSOSFileDevice.h"
#include "MFSSparseDevice.h"


class MFSOSSparseFileDevice
    : public MFSOSFileDevice, public MFSSparseDevice
{
public:
    MFSOSSparseFileDevice(HANDLE hFile, bool readonly);

    uint64_t GetActualAllocatedSize() const;

    bool ZeroRegion(uint64_t startOffset, uint64_t length) override;
    bool ZeroAll() override;
    bool SupportSparse() const override;

private:
    uint64_t _actualSize;
    bool _supportSparse;
};
