#pragma once

#include "MFSStream.h"
#include "../MFSString.h"


/*

class MFSStreamTextReader
提供一组格式化读取文本流内容的帮助函数。

    构造器：

    MFSStreamTextWriter::MFSStreamTextWriter(MFSStream * stream)
        从给定的流对象初始化一个新的 MFSStreamTextWriter 对象。

    成员函数：

    MFSStream * MFSStreamTextWriter::GetStream() const
        获取基础流对象。

    template <typename IntegerT> void MFSStreamTextWriter::WriteInteger(IntegerT value)
        将一个整数的字符串表示写入到基础流中。

    void MFSStreamTextWriter::Write(const MFSString & string)
        将一个字符串写入基础流中。

    void MFSStreamTextWriter::WriteChar(wchar_t ch)
        将一个字符写入基础流中。

    void MFSStreamTextWriter::WriteLine(const MFSString & string)
        将一个字符串写入基础流中，然后写入一个换行符。

*/

class MFSStreamTextWriter
{
public:
    MFSStreamTextWriter(MFSStream * stream);

    MFSStream * GetStream() const;

    template <typename IntegerT>
    void WriteInteger(IntegerT value);

    void Write(const MFSString & string);
    void WriteChar(wchar_t ch);
    void WriteLine(const MFSString & string);

private:
    MFSStream * _stream;
};

template<typename IntegerT>
inline void MFSStreamTextWriter::WriteInteger(IntegerT value)
{
    Write(MFSGetString(value));
}
