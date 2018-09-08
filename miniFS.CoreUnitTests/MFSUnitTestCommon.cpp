#include "stdafx.h"

#define MFS_UNIT_TEST_FILENAME        L"MFSPartitionFormatTest.dat"



HANDLE MFSUnitTestOpenPartitionFileHandle(bool openExisting)
{
    HANDLE hFile = CreateFileW(
        MFS_UNIT_TEST_FILENAME,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_DELETE,
        NULL,
        openExisting ? OPEN_EXISTING : CREATE_ALWAYS,
        // FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MFSUnitTestLogLastError(L"CreateFileW failed");
        Assert::Fail();
    }
    return hFile;
}


void MFSUnitTestLogLastError(const wchar_t * msg)
{
    DWORD errCode = GetLastError();

    DWORD strBufferSize = MAX_PATH + 10;
    std::unique_ptr<WCHAR[]> strBuffer(new WCHAR[strBufferSize]);

    swprintf_s(strBuffer.get(),
        strBufferSize,
        L"ERR: %s. GetLastError() = %u: ",
        msg,
        static_cast<uint32_t>(errCode));
    Logger::WriteMessage(strBuffer.get());

    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
        errCode,
        0,
        strBuffer.get(),
        strBufferSize,
        NULL);
    Logger::WriteMessage(strBuffer.get());
}