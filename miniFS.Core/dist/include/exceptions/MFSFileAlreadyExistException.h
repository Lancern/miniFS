#pragma once

#include "MFSException.h"


class MFSFileAlreadyExistException
    : public MFSException
{
public:
    MFSFileAlreadyExistException();
    MFSFileAlreadyExistException(const MFSString & messeag);
};
