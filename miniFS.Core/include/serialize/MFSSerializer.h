#pragma once

#include "../stream/MFSStream.h"


/*

template <typename T> class MFSSerializer
为对象序列化器提供抽象基类。

    成员函数：

    void MFSSerializer::Serialize(MFSStream * stream, T * object)
        将一个对象序列化入给定的流中。给定的流对象需要有写权限。

    T * MFSSerializer::Deserialize(MFSStream * stream)
        从流中反序列化出一个对象。给定的流对象需要有读权限。

*/


template <typename T>
class MFSSerializer
{
public:
    virtual void Serialize(MFSStream * stream, T * object) = 0;
    virtual T * Deserialize(MFSStream * stream) = 0;
};
