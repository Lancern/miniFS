#include "../../include/exceptions/MFSFileNotFoundException.h"


MFSFileNotFoundException::MFSFileNotFoundException()
	: MFSFileNotFoundException(L"Data space not loaded.")
{
}

MFSFileNotFoundException::MFSFileNotFoundException(const MFSString & message)
	: MFSException(message)
{
}
