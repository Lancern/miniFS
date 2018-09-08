#include "../../include/device/MFSBlockDevice.h"
#include <cstring>



MFSBlockDevice::MFSBlockDevice(MFSRawDevice * rawDevice)
    : _rawDevice(rawDevice)
{
}


MFSRawDevice * MFSBlockDevice::GetRawDevice() const
{
    return _rawDevice;
}

uint32_t MFSBlockDevice::GetBlockSize() const
{
    return 4 * 1024;
}

uint64_t MFSBlockDevice::GetBlocksCount() const
{
    uint64_t dwCbSize = _rawDevice->GetTotalSize();
    uint32_t dwBlockSize = GetBlockSize();
    uint32_t remainder = dwCbSize % dwBlockSize;
    return dwCbSize / dwBlockSize + (remainder ? 1 : 0);
}

bool MFSBlockDevice::ReadBlock(void * lpBuffer, uint64_t blockId)
{
    if (blockId < 0 || blockId >= GetBlocksCount())
        return false;

    MFSRawDeviceView * blockView = OpenBlockView(blockId, true);
    if (!blockView)
        return false;

    uint32_t blockSize = GetBlockSize();
    memset(lpBuffer, 0, blockSize);
    uint32_t read = blockView->Read(lpBuffer, 0, blockSize);

    blockView->Close();
    delete blockView;

    return read;
}

bool MFSBlockDevice::WriteBlock(uint64_t blockId, const void * lpBuffer)
{
    if (blockId < 0 || blockId >= GetBlocksCount())
        return false;

    MFSRawDeviceView * blockView = OpenBlockView(blockId, false);
    if (!blockView)
        return false;

    uint32_t write = blockView->Write(0, GetBlockSize(), lpBuffer);
    blockView->Flush();
    blockView->Close();
    delete blockView;

    return write;
}

void MFSBlockDevice::Close()
{
}

MFSRawDeviceView * MFSBlockDevice::OpenBlockView(uint64_t blockId, bool readonly)
{
    uint32_t blockSize = GetBlockSize();
    uint64_t offset = blockId * blockSize;
    return _rawDevice->OpenView(offset, blockSize, readonly);
}
