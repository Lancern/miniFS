#pragma once

#include "MFSException.h"


class MFSInvalidOperationException
    : public MFSException
{
public:
    MFSInvalidOperationException();
    MFSInvalidOperationException(const MFSString & message);
};
