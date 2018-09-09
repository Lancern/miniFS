#include "../../include/exceptions/MFSInvalidPathException.h"


MFSInvalidPathException::MFSInvalidPathException()
	: MFSInvalidPathException(L"Data space not loaded.")
{
}

MFSInvalidPathException::MFSInvalidPathException(const MFSString & message)
	: MFSException(message)
{
}
