#pragma once

#include "MFSException.h"


class MFSFileNotFoundException
    : public MFSException
{
public:
    MFSFileNotFoundException(const MFSString & filename);
};
