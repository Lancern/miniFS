#include "../../include/exceptions/MFSFileAlreadyExistException.h"


MFSFileAlreadyExistException::MFSFileAlreadyExistException()
	: MFSFileAlreadyExistException(L"Data space not loaded.")
{
}

MFSFileAlreadyExistException::MFSFileAlreadyExistException(const MFSString & message)
	: MFSException(message)
{
}
