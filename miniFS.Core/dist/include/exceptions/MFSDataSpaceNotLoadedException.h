#pragma once

#include "MFSException.h"


/*

class MFSDataSpaceNotLoadedException
Ϊ���ݿռ�δ�����쳣�ṩ���ݡ�

*/

class MFSDataSpaceNotLoadedException
    : public MFSException
{
public:
    MFSDataSpaceNotLoadedException();
    MFSDataSpaceNotLoadedException(const MFSString & message);
};
