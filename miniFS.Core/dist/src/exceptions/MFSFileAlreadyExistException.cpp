#include "../../include/exceptions/MFSFileAlreadyExistException.h"


MFSFileAlreadyExistException::MFSFileAlreadyExistException(const MFSString & filename)
	: MFSException(MFSString::Format(L"File %s already exist.", filename.GetRawString()))
{
}
