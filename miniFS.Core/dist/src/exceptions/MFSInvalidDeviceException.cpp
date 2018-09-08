#include "../../include/exceptions/MFSInvalidDeviceException.h"


MFSInvalidDeviceException::MFSInvalidDeviceException()
	: MFSInvalidDeviceException(L"Data space not loaded.")
{
}

MFSInvalidDeviceException::MFSInvalidDeviceException(const MFSString & message)
	: MFSException(message)
{
}
