#include "../../include/exceptions/MFSInvalidArgumentException.h"


MFSInvalidArgumentException::MFSInvalidArgumentException()
	: MFSInvalidArgumentException(L"Data space not loaded.")
{
}

MFSInvalidArgumentException::MFSInvalidArgumentException(const MFSString & message)
	: MFSException(message)
{
}
