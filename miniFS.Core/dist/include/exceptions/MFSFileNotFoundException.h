#pragma once

#include "MFSException.h"


class MFSFileNotFoundException
    : public MFSException
{
public:
    MFSFileNotFoundException();
    MFSFileNotFoundException(const MFSString & message);
};
