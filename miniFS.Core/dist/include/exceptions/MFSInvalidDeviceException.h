#pragma once

#include "MFSException.h"


class MFSInvalidDeviceException
    : public MFSException
{
public:
    MFSInvalidDeviceException();
    MFSInvalidDeviceException(const MFSString & message);
};
