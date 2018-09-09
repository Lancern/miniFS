#include "../../include/exceptions/MFSWindowsException.h"
#include <Windows.h>


MFSWindowsException::MFSWindowsException()
    : MFSWindowsException(GetLastError())
{
}

MFSWindowsException::MFSWindowsException(uint32_t errCode)
	: _errCode(errCode)
{
    wchar_t * lpMsgBuf;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&lpMsgBuf),
        0, NULL
    );

    SetExceptMessage(lpMsgBuf);
    LocalFree(lpMsgBuf);
}

uint32_t MFSWindowsException::GetErrorCode() const noexcept
{
	return _errCode;
}
