#pragma once

#include "../../../include/MFSString.h"



/*

class MFSException
Ϊ mini-FS �е��쳣���ṩ���ࡣ

    ��������

    MFSException()
        Ĭ�Ϲ��졣

    MFSException(const MFSString & message)
        ʹ��ָ�����쳣��Ϣ��ʼ�� MFSException �����ʵ����

*/


class MFSException
{
public:
    MFSException(const MFSString & message);
    MFSException();

    MFSString GetExceptMessage() const;

private:
    MFSString _msg;
};
