#include "../../include/exceptions/MFSDirectoryNotFoundException.h"


MFSDirectoryNotFoundException::MFSDirectoryNotFoundException(const MFSString & directory)
	: MFSException(MFSString::Format(L"Directory %s cannot be found.", directory))
{
}
