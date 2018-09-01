#include "../../include/stream/MFSMemoryStream.h"

MFSMemoryStream::MFSMemoryStream(LPVOID lpBuffer, DWORD bufferSize)
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

UINT64 MFSMemoryStream::GetLength() const
{
    return _size;
}

UINT64 MFSMemoryStream::GetPosition() const
{
    return _offset;
}

DWORD MFSMemoryStream::Read(LPVOID lpBuffer, DWORD dwBufferSize, DWORD dwNumberOfBytesToRead)
{
    if (dwBufferSize < dwNumberOfBytesToRead)
        dwNumberOfBytesToRead = dwBufferSize;

    DWORD read = 0;
    const UCHAR * src = reinterpret_cast<const UCHAR *>(_lpBuffer);
    UCHAR * dest = reinterpret_cast<UCHAR *>(lpBuffer);

    while (_offset < _size && read < dwNumberOfBytesToRead)
    {
        *dest++ = src[_offset++];
        ++read;
    }
    
    return read;
}

DWORD MFSMemoryStream::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite)
{
    DWORD write = 0;
    const UCHAR * src = reinterpret_cast<const UCHAR *>(lpBuffer);
    UCHAR * dest = reinterpret_cast<UCHAR *>(_lpBuffer);

    while (write < dwNumberOfBytesToWrite && _offset < _size)
    {
        dest[_offset++] = *src++;
        ++write;
    }

    return write;
}

bool MFSMemoryStream::Seek(MFSStreamSeekOrigin origin, INT64 offset)
{
    switch (origin)
    {
    case MFSStreamSeekOrigin::Begin:
        _offset = (offset < 0 ? 0 : static_cast<DWORD>(offset));
        break;
    case MFSStreamSeekOrigin::Relative:
        if (offset < 0 && _offset < -offset)
            _offset = 0;
        else
            _offset += static_cast<DWORD>(offset);
        break;
    case MFSStreamSeekOrigin::End:
        if (offset < 0 && _size < -offset)
            _offset = 0;
        else
            _offset = _size + static_cast<DWORD>(offset);
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
