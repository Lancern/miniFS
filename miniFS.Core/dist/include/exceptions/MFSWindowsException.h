#pragma once

#include "MFSException.h"


class MFSWindowsException
    : public MFSException
{
public:
    MFSWindowsException();
    MFSWindowsException(uint32_t errCode);

	uint32_t GetErrorCode() const noexcept;

private:
	uint32_t _errCode;
};
