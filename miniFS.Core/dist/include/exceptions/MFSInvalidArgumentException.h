#pragma once

#include "MFSException.h"


class MFSInvalidArgumentException
    : public MFSException
{
public:
    MFSInvalidArgumentException();
    MFSInvalidArgumentException(const MFSString & message);
};
