#pragma once

#include "MFSException.h"


/*

class MFSInvalidPathException
Ϊ��Ч·���쳣�ṩ���ݡ�

*/

class MFSInvalidPathException
    : public MFSException
{
public:
    MFSInvalidPathException();
    MFSInvalidPathException(const MFSString & message);
};