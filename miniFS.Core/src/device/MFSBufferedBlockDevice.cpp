#include "../../include/device/MFSBufferedBlockDevice.h"



MFSBufferedBlockDevice::MFSBufferedBlockDevice(MFSRawDevice * rawDevice, DWORD bufferedBlockCount)
    : MFSBlockDevice(rawDevice), _wreq(), _bufferedBlockCount(bufferedBlockCount)
{
}


MFSBufferedBlockDevice::~MFSBufferedBlockDevice()
{
    Close();
}


bool MFSBufferedBlockDevice::ReadBlock(LPVOID lpBuffer, UINT64 blockId)
{
    if (blockId < 0 || blockId >= GetBlocksCount())
        return false;

    MFSRawDeviceView * blockView = nullptr;
    bool fromBuffer = (_wreq.find(blockId) != _wreq.end());

    if (_wreq.find(blockId) != _wreq.end())
        blockView = _wreq.find(blockId)->second;
    else
        blockView = OpenBlockView(blockId, true);

    if (!blockView)
        return false;

    DWORD read = blockView->Read(lpBuffer, 0, GetBlockSize());

    if (!fromBuffer) 
    {
        blockView->Close();
        delete blockView;
    }

    return read;
}

bool MFSBufferedBlockDevice::WriteBlock(UINT64 blockId, LPCVOID lpBuffer)
{
    if (blockId < 0 || blockId >= GetBlocksCount())
        return false;

    MFSRawDeviceView * blockView = LoadBlockIntoBuffer(blockId);
    if (!blockView)
        return false;

    DWORD write = blockView->Write(0, GetBlockSize(), lpBuffer);
    return write;
}


void MFSBufferedBlockDevice::Flush()
{
    for (const auto & bufferItem : _wreq)
    {
        bufferItem.second->Flush();
        bufferItem.second->Close();
    }
    _wreq.clear();
}

void MFSBufferedBlockDevice::Close()
{
    Flush();
    MFSBlockDevice::Close();
}

MFSRawDeviceView * MFSBufferedBlockDevice::LoadBlockIntoBuffer(UINT64 blockId)
{
    if (_wreq.find(blockId) == _wreq.end())
    {
        if (_wreq.size() >= _bufferedBlockCount)
        {
            Flush();
        }

        MFSRawDeviceView * blockView = OpenBlockView(blockId, false);
        _wreq[blockId] = blockView;
    }

    return _wreq.find(blockId)->second;
}
