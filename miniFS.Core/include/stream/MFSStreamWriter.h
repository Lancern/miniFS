#pragma once

#include "MFSStream.h"
#include "../MFSString.h"
#include <type_traits>


/*

class MFSStreamWriter
提供一组格式化写入流数据的帮助函数。

    构造器：

    MFSStreamWriter::MFSStreamWriter(MFSStream * stream)
        使用指定的基础流对象初始化 MFSStreamWriter 类的新实例。

    成员函数：

    MFSStream * GetStream() const
        获取基础流对象。

    template <typename PODType> void MFSStreamWriter::Write(PODType object)
        将一个 POD 类型的对象写入基础流。

    void MFSStreamWriter::Write(const MFSString & string)
        将一个字符串写入基础流。

*/

class MFSStreamWriter
{
public:
    MFSStreamWriter(MFSStream * stream);

    MFSStream * GetStream() const;

    template <typename PODType>
    void Write(PODType object);

    void Write(const MFSString & string);

private:
    MFSStream * _stream;
};

template<typename PODType>
inline void MFSStreamWriter::Write(PODType object)
{
    static_assert()

    _stream->Write(&object, sizeof(object));
}


