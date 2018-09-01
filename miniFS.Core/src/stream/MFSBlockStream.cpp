#include "../../include/stream/MFSBlockStream.h"

MFSBlockStream::MFSBlockStream(MFSBlockDevice * device)
    : _device(device), _insideOffset(0), _blockOffset(0)
{
    BYTE * buffer = new BYTE[device->GetBlockSize()];
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
    return _blockOffset < _device->GetBlocksCount();
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
    if (dwNumberOfBytesToRead > dwBufferSize)
        dwNumberOfBytesToRead = dwBufferSize;

    DWORD read = 0;
    BYTE * dest = reinterpret_cast<BYTE *>(lpBuffer);
    while (read < dwNumberOfBytesToRead && TryReadByte(dest))
    {
        ++dest;
        ++read;
    }

    return read;
}

DWORD MFSBlockStream::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite)
{
    DWORD write = 0;
    const BYTE * src = reinterpret_cast<const BYTE *>(lpBuffer);

    while (write < dwNumberOfBytesToWrite && TryWriteByte(*src))
    {
        ++src;
        ++write;
    }

    return 0;
}

bool MFSBlockStream::Seek(MFSStreamSeekOrigin origin, INT64 offset)
{
    switch (origin)
    {
    case MFSStreamSeekOrigin::Begin:
        if (offset < 0)
            return false;
        else
            return SeekBegin(offset);
    case MFSStreamSeekOrigin::Relative:
        return SeekBegin(GetPosition() + offset);
    case MFSStreamSeekOrigin::End:
        if (offset > 0)
            return false;
        else
            return SeekBegin(GetLength() + offset);
    default:
        return false;
    }
}

void MFSBlockStream::Flush()
{
    if (_buffer && _blockOffset < _device->GetBlocksCount())
    {
        _device->WriteBlock(_blockOffset, _buffer.get());
    }
}

void MFSBlockStream::Close()
{
    if (_device)
    {
        Flush();
        _device = NULL;
        _buffer.release();
    }
}

bool MFSBlockStream::SeekBlock(UINT64 blockId)
{
    if (blockId > _device->GetBlocksCount())
        return false;
    else if (blockId == _device->GetBlocksCount())
    {
        _blockOffset = blockId;
        _insideOffset = 0;
        return true;
    }
    else
    {
        if (blockId != _blockOffset)
        {
            Flush();
            _device->ReadBlock(_buffer.get(), blockId);
        }

        _blockOffset = blockId;
        _insideOffset = 0;

        return true;
    }
}

UINT64 MFSBlockStream::GetCurrentBlockId() const
{
    return _blockOffset;
}

UINT64 MFSBlockStream::GetBlockInternalOffset() const
{
    return _insideOffset;
}

UINT64 MFSBlockStream::OnBlockSwap(UINT64 currentBlock)
{
    return currentBlock + 1;
}

bool MFSBlockStream::SeekBegin(INT64 offset)
{
    DWORD blockSize = _device->GetBlockSize();
    UINT64 blockId = offset / blockSize;
    
    if (!SeekBlock(blockId))
        return false;
    else
    {
        DWORD insideOffset = static_cast<DWORD>(offset % blockSize);
        _insideOffset = insideOffset;

        return true;
    }
}

bool MFSBlockStream::TryReadByte(BYTE * buffer)
{
    if (!HasNext())
        return false;
    
    *buffer = _buffer[_insideOffset++];
    if (_insideOffset == _device->GetBlockSize())
        return SeekBlock(OnBlockSwap(_blockOffset));
    else
        return true;
}

bool MFSBlockStream::TryWriteByte(BYTE data)
{
    if (!HasNext())
        return false;

    _buffer[_insideOffset++] = data;
    if (_insideOffset == _device->GetBlockSize())
        return SeekBlock(OnBlockSwap(_blockOffset));
    else
        return true;
}
