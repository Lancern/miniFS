#pragma once

#include "MFSException.h"


/*

class MFSDataSpaceNotLoadedException
为数据空间未挂载异常提供数据。

*/

class MFSDataSpaceNotLoadedException
    : public MFSException
{
public:
    MFSDataSpaceNotLoadedException();
    MFSDataSpaceNotLoadedException(const MFSString & message);
};
