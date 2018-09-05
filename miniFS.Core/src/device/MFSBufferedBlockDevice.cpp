#include "../../include/device/MFSBufferedBlockDevice.h"



MFSBufferedBlockDevice::MFSBufferedBlockDevice(MFSRawDevice * rawDevice, uint32_t bufferedBlockCount)
    : MFSBlockDevice(rawDevice), _wreq(), _bufferedBlockCount(bufferedBlockCount)
{
}


MFSBufferedBlockDevice::~MFSBufferedBlockDevice()
{
    Close();
}


bool MFSBufferedBlockDevice::ReadBlock(void * lpBuffer, uint64_t blockId)
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

    uint32_t read = blockView->Read(lpBuffer, 0, GetBlockSize());

    if (!fromBuffer) 
    {
        blockView->Close();
        delete blockView;
    }

    return read;
}

bool MFSBufferedBlockDevice::WriteBlock(uint64_t blockId, const void * lpBuffer)
{
    if (blockId < 0 || blockId >= GetBlocksCount())
        return false;

    MFSRawDeviceView * blockView = LoadBlockIntoBuffer(blockId);
    if (!blockView)
        return false;

    uint32_t write = blockView->Write(0, GetBlockSize(), lpBuffer);
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

MFSRawDeviceView * MFSBufferedBlockDevice::LoadBlockIntoBuffer(uint64_t blockId)
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
