#pragma once

#include "../../../include/MFSString.h"



/*

class MFSException
为 mini-FS 中的异常类提供基类。

    构造器：

    MFSException()
        默认构造。

    MFSException(const MFSString & message)
        使用指定的异常消息初始化 MFSException 类的新实例。

*/


class MFSException
{
public:
    MFSException(const MFSString & message);
    MFSException();

    MFSString GetExceptMessage() const;

protected:
    void SetExceptMessage(const MFSString & message) noexcept;

private:
    MFSString _msg;
};
