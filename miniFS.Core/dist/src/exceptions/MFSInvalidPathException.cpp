#include "../../include/exceptions/MFSInvalidPathException.h"


MFSInvalidPathException::MFSInvalidPathException(const MFSString & path)
	: MFSException(MFSString::Format(L"Path %s is invalid.", path.GetRawString()))
	//: MFSException(MFSString::Format(L"%s", path.GetRawString()))
{
}
