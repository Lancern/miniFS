#pragma once

#include "MFSStream.h"
#include "../MFSString.h"
#include <type_traits>


/*

class MFSStreamWriter
�ṩһ���ʽ��д�������ݵİ���������

    ��������

    MFSStreamWriter::MFSStreamWriter(MFSStream * stream)
        ʹ��ָ���Ļ����������ʼ�� MFSStreamWriter �����ʵ����

    ��Ա������

    MFSStream * GetStream() const
        ��ȡ����������

    template <typename PODType> void MFSStreamWriter::Write(PODType object)
        ��һ�� POD ���͵Ķ���д���������

    void MFSStreamWriter::Write(const MFSString & string)
        ��һ���ַ���д���������

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


