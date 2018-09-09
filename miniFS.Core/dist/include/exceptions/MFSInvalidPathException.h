#pragma once

#include "MFSException.h"


/*

class MFSInvalidPathException
为无效路径异常提供数据。

*/

class MFSInvalidPathException
    : public MFSException
{
public:
    MFSInvalidPathException(const MFSString & path);
};