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

UINT64 MFSNativeFileStream::GetLength() const
{
    LARGE_INTEGER size;
    size.QuadPart = 0;

    GetFileSizeEx(_hFile, &size);
    return size.QuadPart;
}

UINT64 MFSNativeFileStream::GetPosition() const
{
    LARGE_INTEGER ptr;
    ptr.QuadPart = 0;

    LARGE_INTEGER offset;
    offset.QuadPart = 0;
    SetFilePointerEx(_hFile, offset, &ptr, FILE_CURRENT);

    return ptr.QuadPart;
}

DWORD MFSNativeFileStream::Read(LPVOID lpBuffer, DWORD dwBufferSize, DWORD dwNumberOfBytesToRead)
{
    DWORD read = 0;
    if (!ReadFile(_hFile, lpBuffer, dwNumberOfBytesToRead, &read, NULL))
        return 0;
    return read;
}

DWORD MFSNativeFileStream::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite)
{
    DWORD write = 0;
    if (!WriteFile(_hFile, lpBuffer, dwNumberOfBytesToWrite, &write, NULL))
        return 0;
    return write;
}

bool MFSNativeFileStream::Seek(MFSStreamSeekOrigin origin, INT64 offset)
{
    DWORD relative;
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
