#pragma once

#include "MFSException.h"


class MFSFileAlreadyExistException
    : public MFSException
{
public:
    MFSFileAlreadyExistException(const MFSString & filename);
};
