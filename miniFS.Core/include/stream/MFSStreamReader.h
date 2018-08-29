#pragma once

#include "MFSStream.h"
#include "../MFSString.h"
#include <type_traits>


/*

class MFSStreamReader
提供一组格式化读取流内容的帮助函数。

    构造器：

    MFSStreamReader::MFSStreamReader(MFSStream * stream)
        从给定的流对象初始化一个新的 MFSStreamReader 对象。

    成员函数：

    MFSStream * GetStream() const
        获取基础流对象。

    template <typename PODType> PODType MFSStreamReader::ReadPODObject()
        从基础流中读取出一个 PODType 类型的对象。
        @typeparam PODType 一个 POD 类型。

    MFSString ReadString(const std::vector<WCHAR> & terminators)
        从基础流中读取出一个以给定的结束符结尾的字符串。

    MFSString ReadString()
        从基础流中读取出一个以 0 结束符结尾的字符串。

    MFSString ReadLine()
        从基础流中读取出一行字符串。行末换行符会被保留在返回的字符串中。

*/

class MFSStreamReader
{
public:
    MFSStreamReader(MFSStream * stream);

    MFSStream * GetStream() const;

    template <typename PODType>
    PODType ReadPODObject();

    MFSString ReadString(const std::vector<WCHAR> & terminators);
    MFSString ReadString();
    MFSString ReadLine();
    
private:
    MFSStream * _stream;
};

template<typename PODType>
inline PODType MFSStreamReader::ReadPODObject()
{
    static_assert(std::is_pod_v<PODType>, "Type given is not a valid POD type.");

    PODType result;
    _stream->Read(&result, sizeof(result), sizeof(result));
    return result;
}
