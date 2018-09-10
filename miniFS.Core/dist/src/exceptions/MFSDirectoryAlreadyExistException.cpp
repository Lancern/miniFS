#include "../../include/exceptions/MFSDirectoryAlreadyExistException.h"


MFSDirectoryAlreadyExistException::MFSDirectoryAlreadyExistException(const MFSString & directory)
    : MFSException(MFSString::Format(L"Directory %s already exist.", directory.GetRawString()))
{
}
