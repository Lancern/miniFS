#include "../../include/exceptions/MFSOutOfSpaceException.h"


MFSOutOfSpaceException::MFSOutOfSpaceException()
	: MFSOutOfSpaceException(L"Data space not loaded.")
{
}

MFSOutOfSpaceException::MFSOutOfSpaceException(const MFSString & message)
	: MFSException(message)
{
}
