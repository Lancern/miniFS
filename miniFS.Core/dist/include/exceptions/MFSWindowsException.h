#pragma once

#include "MFSException.h"


class MFSWindowsException
    : public MFSException
{
public:
    MFSWindowsException();
    MFSWindowsException(uint32_t errCode);
};
