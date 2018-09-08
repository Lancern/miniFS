#include "../../include/exceptions/MFSException.h"


MFSException::MFSException(const MFSString & message)
    : _msg(message)
{
}

MFSException::MFSException()
    : MFSException(L"An MFS exception occured.")
{
}

MFSString MFSException::GetExceptMessage() const noexcept
{
    return _msg;
}

void MFSException::SetExceptMessage(const MFSString & message) noexcept
{
	_msg = message;
}
