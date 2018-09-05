#include "../../include/stream/MFSNativeFileStream.h"


MFSNativeFileStream::MFSNativeFileStream(HANDLE hFile)
    : _hFile(hFile)
{
}

MFSNativeFileStream::~MFSNativeFileStream()
{
    CloseHandle(_hFile);
}

bool MFSNativeFileStream::CanRead() const
{
    return true;
}

bool MFSNativeFileStream::CanWrite() const
{
    return true;
}

bool MFSNativeFileStream::CanSeek() const
{
    return true;
}

bool MFSNativeFileStream::HasNext() const
{
    return GetPosition() < GetLength();
}

uint64_t MFSNativeFileStream::GetLength() const
{
    LARGE_INTEGER size;
    size.QuadPart = 0;

    GetFileSizeEx(_hFile, &size);
    return size.QuadPart;
}

uint64_t MFSNativeFileStream::GetPosition() const
{
    LARGE_INTEGER ptr;
    ptr.QuadPart = 0;

    LARGE_INTEGER offset;
    offset.QuadPart = 0;
    SetFilePointerEx(_hFile, offset, &ptr, FILE_CURRENT);

    return ptr.QuadPart;
}

uint32_t MFSNativeFileStream::Read(void * lpBuffer, uint32_t dwBufferSize, uint32_t dwNumberOfBytesToRead)
{
    DWORD read = 0;
    if (!ReadFile(_hFile, lpBuffer, dwNumberOfBytesToRead, &read, NULL))
        return 0;
    return static_cast<uint32_t>(read);
}

uint32_t MFSNativeFileStream::Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite)
{
    DWORD write = 0;
    if (!WriteFile(_hFile, lpBuffer, dwNumberOfBytesToWrite, &write, NULL))
        return 0;
    return static_cast<uint32_t>(write);
}

bool MFSNativeFileStream::Seek(MFSStreamSeekOrigin origin, int64_t offset)
{
    uint32_t relative;
    switch (origin)
    {
    case MFSStreamSeekOrigin::Begin:
        relative = FILE_BEGIN;
        break;
    case MFSStreamSeekOrigin::End:
        relative = FILE_END;
        break;
    case MFSStreamSeekOrigin::Relative:
    default:
        relative = FILE_CURRENT;
        break;
    }

    LARGE_INTEGER liOffset;
    liOffset.QuadPart = offset;
    return static_cast<bool>(SetFilePointerEx(_hFile, liOffset, NULL, relative));
}

void MFSNativeFileStream::Flush()
{
    if (_hFile)
        FlushFileBuffers(_hFile);
}

void MFSNativeFileStream::Close()
{
    if (_hFile)
    {
        Flush();
        CloseHandle(_hFile);
        _hFile = NULL;
    }
}
