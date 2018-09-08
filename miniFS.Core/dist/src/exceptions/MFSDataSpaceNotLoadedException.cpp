#include "../../include/exceptions/MFSDataSpaceNotLoadedException.h"


MFSDataSpaceNotLoadedException::MFSDataSpaceNotLoadedException()
    : MFSDataSpaceNotLoadedException(L"Data space not loaded.")
{
}

MFSDataSpaceNotLoadedException::MFSDataSpaceNotLoadedException(const MFSString & message)
    : MFSException(message)
{
}
