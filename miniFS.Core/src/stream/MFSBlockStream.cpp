#include "../../include/stream/MFSBlockStream.h"

MFSBlockStream::MFSBlockStream(MFSBlockDevice * device)
    : _device(device), _insideOffset(0), _blockOffset(0), _dirty(false)
{
    uint8_t * buffer = new uint8_t[device->GetBlockSize()];
    _buffer.reset(buffer);

    // Load first block.
    device->ReadBlock(buffer, 0);
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

uint64_t MFSBlockStream::GetLength() const
{
    return _device->GetBlocksCount() * _device->GetBlockSize();
}

uint64_t MFSBlockStream::GetPosition() const
{
    return static_cast<uint64_t>(_blockOffset) * _device->GetBlockSize() + _insideOffset;
}

MFSBlockDevice * MFSBlockStream::GetDevice() const
{
    return _device;
}

uint32_t MFSBlockStream::GetDeviceBlockSize() const
{
    return _device->GetBlockSize();
}

uint64_t MFSBlockStream::GetDeviceBlocksCount() const
{
    return _device->GetBlocksCount();
}

uint32_t MFSBlockStream::Read(void * lpBuffer, uint32_t dwBufferSize, uint32_t dwNumberOfBytesToRead)
{
    if (dwNumberOfBytesToRead > dwBufferSize)
        dwNumberOfBytesToRead = dwBufferSize;

    uint32_t read = 0;
    uint8_t * dest = reinterpret_cast<uint8_t *>(lpBuffer);
    while (read < dwNumberOfBytesToRead && TryReadByte(dest))
    {
        ++dest;
        ++read;
    }

    return read;
}

uint32_t MFSBlockStream::Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite)
{
    uint32_t write = 0;
    const uint8_t * src = reinterpret_cast<const uint8_t *>(lpBuffer);

    while (write < dwNumberOfBytesToWrite && TryWriteByte(*src))
    {
        ++src;
        ++write;
    }

    return 0;
}

bool MFSBlockStream::Seek(MFSStreamSeekOrigin origin, int64_t offset)
{
    switch (origin)
    {
    case MFSStreamSeekOrigin::Begin:
        if (offset < 0)
            return false;
        else
            return SeekFromBegin(offset);
    case MFSStreamSeekOrigin::Relative:
        return SeekFromBegin(GetPosition() + offset);
    case MFSStreamSeekOrigin::End:
        if (offset > 0)
            return false;
        else
            return SeekFromBegin(GetLength() + offset);
    default:
        return false;
    }
}

void MFSBlockStream::Flush()
{
    if (_buffer && _blockOffset < _device->GetBlocksCount() && _dirty)
    {
        _device->WriteBlock(_blockOffset, _buffer.get());
        _dirty = false;
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

bool MFSBlockStream::SeekBlock(uint64_t blockId)
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

bool MFSBlockStream::IsDirty() const
{
    return _dirty;
}

void MFSBlockStream::SetDirtyFlag(bool dirty)
{
    _dirty = dirty;
}

uint64_t MFSBlockStream::GetCurrentBlockId() const
{
    return _blockOffset;
}

uint64_t MFSBlockStream::GetBlockInternalOffset() const
{
    return _insideOffset;
}

uint64_t MFSBlockStream::OnBlockSwap(uint64_t currentBlock)
{
    return currentBlock + 1;
}

bool MFSBlockStream::SeekFromBegin(int64_t offset)
{
    uint32_t blockSize = _device->GetBlockSize();
    uint64_t blockId = offset / blockSize;
    
    if (!SeekBlock(blockId))
        return false;
    else
    {
        uint32_t insideOffset = static_cast<uint32_t>(offset % blockSize);
        _insideOffset = insideOffset;

        return true;
    }
}

bool MFSBlockStream::TryReadByte(uint8_t * buffer)
{
    if (!HasNext())
        return false;
    
    *buffer = _buffer[_insideOffset++];
    if (_insideOffset != _device->GetBlockSize())
        return true;
    else
    {
        Flush();
        return SeekBlock(OnBlockSwap(_blockOffset));
    }
}

bool MFSBlockStream::TryWriteByte(uint8_t data)
{
    if (!HasNext())
        return false;

    _buffer[_insideOffset++] = data;
    _dirty = true;

    if (_insideOffset != _device->GetBlockSize())
        return true;
    else
    {
        Flush();
        return SeekBlock(OnBlockSwap(_blockOffset));
    }
}
