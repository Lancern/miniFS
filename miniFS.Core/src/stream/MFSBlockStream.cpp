#include "../../include/stream/MFSBlockStream.h"

MFSBlockStream::MFSBlockStream(MFSBlockDevice * device)
    : _device(device), _insideOffset(0), _blockOffset(0)
{
    UCHAR * buffer = new UCHAR[device->GetBlockSize()];
    _buffer.reset(buffer);
}

bool MFSBlockStream::CanRead() const
{
    return true;
}

bool MFSBlockStream::CanWrite() const
{
    return true;
}

bool MFSBlockStream::CanSeek() const
{
    return true;
}

bool MFSBlockStream::HasNext() const
{
    return _insideOffset < _device->GetBlockSize() || 
           _blockOffset < _device->GetBlocksCount();
}

UINT64 MFSBlockStream::GetLength() const
{
    return _device->GetBlocksCount() * _device->GetBlockSize();
}

UINT64 MFSBlockStream::GetPosition() const
{
    return static_cast<UINT64>(_blockOffset) * _device->GetBlockSize() + _insideOffset;
}

MFSBlockDevice * MFSBlockStream::GetDevice() const
{
    return _device;
}

DWORD MFSBlockStream::GetDeviceBlockSize() const
{
    return _device->GetBlockSize();
}

UINT64 MFSBlockStream::GetDeviceBlocksCount() const
{
    return _device->GetBlocksCount();
}

DWORD MFSBlockStream::Read(LPVOID lpBuffer, DWORD dwBufferSize, DWORD dwNumberOfBytesToRead)
{
    // TODO: Implement MFSBlockStream::Read
    return 0;
}

DWORD MFSBlockStream::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite)
{
    // TODO: Implement MFSBlockStream::Write
    return 0;
}

bool MFSBlockStream::Seek(MFSStreamSeekOrigin origin, INT64 offset)
{
    // TODO: Implement MFSBlockStream::Seek.
    return false;
}

void MFSBlockStream::Flush()
{
    // TODO: Implement MFSBlockStream::Flush.
}

void MFSBlockStream::Close()
{
    // TODO: Implement MFSBlockStream::Close.
}
