#pragma once

#include "MFSException.h"


class MFSInvalidEntryTypeException
    : public MFSException
{
public:
    MFSInvalidEntryTypeException(const MFSString & entryPath);
};
