#include "../../include/stream/MFSMemoryStream.h"


MFSMemoryStream::MFSMemoryStream(void * lpBuffer, uint32_t bufferSize)
    : _lpBuffer(lpBuffer), _size(bufferSize), _offset(0)
{
}

bool MFSMemoryStream::CanRead() const
{
    return true;
}

bool MFSMemoryStream::CanWrite() const
{
    return true;
}

bool MFSMemoryStream::CanSeek() const
{
    return true;
}

bool MFSMemoryStream::HasNext() const
{
    return _offset < _size;
}

uint64_t MFSMemoryStream::GetLength() const
{
    return _size;
}

uint64_t MFSMemoryStream::GetPosition() const
{
    return _offset;
}

uint32_t MFSMemoryStream::Read(void * lpBuffer, uint32_t dwBufferSize, uint32_t dwNumberOfBytesToRead)
{
    if (dwBufferSize < dwNumberOfBytesToRead)
        dwNumberOfBytesToRead = dwBufferSize;

    uint32_t read = 0;
    const uint8_t * src = reinterpret_cast<const uint8_t *>(_lpBuffer);
    uint8_t * dest = reinterpret_cast<uint8_t *>(lpBuffer);

    while (_offset < _size && read < dwNumberOfBytesToRead)
    {
        *dest++ = src[_offset++];
        ++read;
    }
    
    return read;
}

uint32_t MFSMemoryStream::Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite)
{
    uint32_t write = 0;
    const uint8_t * src = reinterpret_cast<const uint8_t *>(lpBuffer);
    uint8_t * dest = reinterpret_cast<uint8_t *>(_lpBuffer);

    while (write < dwNumberOfBytesToWrite && _offset < _size)
    {
        dest[_offset++] = *src++;
        ++write;
    }

    return write;
}

bool MFSMemoryStream::Seek(MFSStreamSeekOrigin origin, int64_t offset)
{
    switch (origin)
    {
    case MFSStreamSeekOrigin::Begin:
        _offset = (offset < 0 ? 0 : static_cast<uint32_t>(offset));
        break;
    case MFSStreamSeekOrigin::Relative:
        if (offset < 0 && _offset < -offset)
            _offset = 0;
        else
            _offset += static_cast<uint32_t>(offset);
        break;
    case MFSStreamSeekOrigin::End:
        if (offset < 0 && _size < -offset)
            _offset = 0;
        else
            _offset = _size + static_cast<uint32_t>(offset);
        break;
    default:
        return false;
    }
    if (_offset > _size)
        _offset = _size;
    return true;
}

void MFSMemoryStream::Flush()
{
}

void MFSMemoryStream::Close()
{
}
