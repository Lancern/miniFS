#include "../../include/exceptions/MFSInvalidOperationException.h"


MFSInvalidOperationException::MFSInvalidOperationException()
    : MFSException(L"Invalid operation.")
{
}

MFSInvalidOperationException::MFSInvalidOperationException(const MFSString & message)
    : MFSException(message)
{
}
