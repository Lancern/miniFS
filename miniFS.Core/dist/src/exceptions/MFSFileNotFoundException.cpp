#include "../../include/exceptions/MFSFileNotFoundException.h"


MFSFileNotFoundException::MFSFileNotFoundException(const MFSString & filename)
	: MFSException(MFSString::Format(L"File %s cannot be found.", filename.GetRawString()))
{
}
