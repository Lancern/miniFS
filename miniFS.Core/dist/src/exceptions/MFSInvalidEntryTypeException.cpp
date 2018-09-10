#include "../../include/exceptions/MFSInvalidEntryTypeException.h"


MFSInvalidEntryTypeException::MFSInvalidEntryTypeException(const MFSString & entryPath)
    : MFSException(MFSString::Format(L"The entry type of %s is invalid.", entryPath.GetRawString()))
{
}
