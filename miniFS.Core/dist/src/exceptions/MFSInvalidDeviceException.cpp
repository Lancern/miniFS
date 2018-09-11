#include "../../include/exceptions/MFSInvalidDeviceException.h"


MFSInvalidDeviceException::MFSInvalidDeviceException()
	: MFSInvalidDeviceException(L"Invalid Device.")
{
}

MFSInvalidDeviceException::MFSInvalidDeviceException(const MFSString & message)
	: MFSException(message)
{
}
