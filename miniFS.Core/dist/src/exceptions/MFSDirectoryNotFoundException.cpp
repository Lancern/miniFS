#include "../../include/exceptions/MFSDirectoryNotFoundException.h"


MFSDirectoryNotFoundException::MFSDirectoryNotFoundException()
	: MFSDirectoryNotFoundException(L"Data space not loaded.")
{
}

MFSDirectoryNotFoundException::MFSDirectoryNotFoundException(const MFSString & message)
	: MFSException(message)
{
}
