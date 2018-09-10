#include "../../include/exceptions/MFSOutOfSpaceException.h"


MFSOutOfSpaceException::MFSOutOfSpaceException()
	: MFSOutOfSpaceException(L"Not enough space.")
{
}

MFSOutOfSpaceException::MFSOutOfSpaceException(const MFSString & message)
	: MFSException(message)
{
}
