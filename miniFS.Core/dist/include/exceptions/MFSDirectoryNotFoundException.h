#pragma once

#include "MFSException.h"


class MFSDirectoryNotFoundException
    : public MFSException
{
public:
    MFSDirectoryNotFoundException(const MFSString & directory);
};
