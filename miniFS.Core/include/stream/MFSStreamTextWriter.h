#pragma once

#include "MFSStream.h"
#include "../MFSString.h"


/*

class MFSStreamTextReader
�ṩһ���ʽ����ȡ�ı������ݵİ���������

    ��������

    MFSStreamTextWriter::MFSStreamTextWriter(MFSStream * stream)
        �Ӹ������������ʼ��һ���µ� MFSStreamTextWriter ����

    ��Ա������

    MFSStream * MFSStreamTextWriter::GetStream() const
        ��ȡ����������

    template <typename IntegerT> void MFSStreamTextWriter::WriteInteger(IntegerT value)
        ��һ���������ַ�����ʾд�뵽�������С�

    void MFSStreamTextWriter::Write(const MFSString & string)
        ��һ���ַ���д��������С�

    void MFSStreamTextWriter::WriteLine(const MFSString & string)
        ��һ���ַ���д��������У�Ȼ��д��һ�����з���

*/

class MFSStreamTextWriter
{
public:
    MFSStreamTextWriter(MFSStream * stream);

    MFSStream * GetStream() const;

    template <typename IntegerT>
    void WriteInteger(IntegerT value);

    void Write(const MFSString & string);
    void WriteLine(const MFSString & string);

private:
    MFSStream * _stream;
};
