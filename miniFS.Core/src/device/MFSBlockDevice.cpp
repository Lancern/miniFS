#include "../../include/device/MFSBlockDevice.h"
#include <cstring>



MFSBlockDevice::MFSBlockDevice(MFSRawDevice * rawDevice)
    : _rawDevice(rawDevice)
{
}

MFSBlockDevice::~MFSBlockDevice()
{
    MFSBlockDevice::Close();
}


MFSRawDevice * MFSBlockDevice::GetRawDevice() const
{
    return _rawDevice;
}

DWORD MFSBlockDevice::GetBlockSize() const
{
    return 4 * 1024;
}

UINT64 MFSBlockDevice::GetBlocksCount() const
{
    UINT64 dwCbSize = _rawDevice->GetTotalSize();
    DWORD dwBlockSize = GetBlockSize();
    DWORD remainder = dwCbSize % dwBlockSize;
    return dwCbSize / dwBlockSize + (remainder ? 1 : 0);
}

bool MFSBlockDevice::ReadBlock(LPVOID lpBuffer, UINT64 blockId)
{
    if (blockId < 0 || blockId >= GetBlocksCount())
        return false;

    MFSRawDeviceView * blockView = OpenBlockView(blockId, true);
    if (!blockView)
        return false;

    DWORD blockSize = GetBlockSize();
    memset(lpBuffer, 0, blockSize);
    DWORD read = blockView->Read(lpBuffer, 0, blockSize);

    blockView->Close();
    delete blockView;

    return read;
}

bool MFSBlockDevice::WriteBlock(UINT64 blockId, LPCVOID lpBuffer)
{
    if (blockId < 0 || blockId >= GetBlocksCount())
        return false;

    MFSRawDeviceView * blockView = OpenBlockView(blockId, false);
    if (!blockView)
        return false;

    DWORD write = blockView->Write(0, GetBlockSize(), lpBuffer);
    blockView->Flush();
    blockView->Close();
    delete blockView;

    return write;
}

void MFSBlockDevice::Close()
{
}

MFSRawDeviceView * MFSBlockDevice::OpenBlockView(UINT64 blockId, bool readonly)
{
    DWORD blockSize = GetBlockSize();
    UINT64 offset = blockId * blockSize;
    return _rawDevice->OpenView(offset, blockSize, readonly);
}
