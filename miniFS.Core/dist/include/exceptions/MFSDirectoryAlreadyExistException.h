#pragma once

#include "MFSException.h"


class MFSDirectoryAlreadyExistException
    : public MFSException
{
public:
    MFSDirectoryAlreadyExistException(const MFSString & directory);
};
