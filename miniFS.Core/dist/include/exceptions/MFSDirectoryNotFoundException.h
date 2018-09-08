#pragma once

#include "MFSException.h"


class MFSDirectoryNotFoundException
    : public MFSException
{
public:
    MFSDirectoryNotFoundException();
    MFSDirectoryNotFoundException(const MFSString & message);
};
