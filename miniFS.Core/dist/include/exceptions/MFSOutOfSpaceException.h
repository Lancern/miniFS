#pragma once

#include "MFSException.h"


class MFSOutOfSpaceException
    : public MFSException
{
public:
    MFSOutOfSpaceException();
    MFSOutOfSpaceException(const MFSString & message);
};
