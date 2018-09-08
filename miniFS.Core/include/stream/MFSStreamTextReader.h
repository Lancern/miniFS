#pragma once

#include "MFSStream.h"
#include "../MFSString.h"
#include <vector>


/*

class MFSStreamTextReader
提供一组格式化读取文本流内容的帮助函数。

    构造器：

    MFSStreamTextReader::MFSStreamTextReader(MFSStream * stream)
        从给定的流对象初始化一个新的 MFSStreamTextReader 对象。

    成员函数：

    MFSStream * MFSStreamTextReader::GetStream() const
        获取基础流对象。

    wchar_t MFSStreamTextReader::ReadChar()
        从基础流中读取一个字符。

    MFSString MFSStreamTextReader::ReadString(const std::vector<wchar_t> & terminators, bool includeTerminator)
        从基础流中读取出一个以给定的结束符结尾的字符串。
        @param includeTerminator 一个 bool 值指示分隔符是否应保留在返回的字符串中。

    MFSString MFSStreamTextReader::ReadToken()
        从基础流中读取出一个以空白字符结尾的字符串。

    MFSString MFSStreamTextReader::ReadLine()
        从基础流中读取出一行字符串。行末换行符会被保留在返回的字符串中。

*/

class MFSStreamTextReader
{
public:
    MFSStreamTextReader(MFSStream * stream);

    MFSStream * GetStream() const;

    wchar_t ReadChar();

    MFSString Read(const std::vector<wchar_t> & terminators, bool includeTerminator);

    MFSString ReadToken();
    MFSString ReadLine();

private:
    MFSStream * _stream;
};
