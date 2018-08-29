#pragma once

#include "../stream/MFSStream.h"


/*

template <typename T> class MFSSerializer
Ϊ�������л����ṩ������ࡣ

    ��Ա������

    void MFSSerializer::Serialize(MFSStream * stream, T * object)
        ��һ���������л�����������С���������������Ҫ��дȨ�ޡ�

    T * MFSSerializer::Deserialize(MFSStream * stream)
        �����з����л���һ�����󡣸�������������Ҫ�ж�Ȩ�ޡ�

*/


template <typename T>
class MFSSerializer
{
public:
    virtual void Serialize(MFSStream * stream, T * object) = 0;
    virtual T * Deserialize(MFSStream * stream) = 0;
};
