#pragma once

#include "MFSStream.h"
#include "../MFSString.h"
#include <type_traits>


/*

class MFSStreamReader
�ṩһ���ʽ����ȡ�����ݵİ���������

    ��������

    MFSStreamReader::MFSStreamReader(MFSStream * stream)
        �Ӹ������������ʼ��һ���µ� MFSStreamReader ����

    ��Ա������

    MFSStream * GetStream() const
        ��ȡ����������

    template <typename PODType> PODType MFSStreamReader::ReadPODObject()
        �ӻ������ж�ȡ��һ�� PODType ���͵Ķ���
        @typeparam PODType һ�� POD ���͡�

    MFSString ReadString(const std::vector<WCHAR> & terminators)
        �ӻ������ж�ȡ��һ���Ը����Ľ�������β���ַ�����

    MFSString ReadString()
        �ӻ������ж�ȡ��һ���� 0 ��������β���ַ�����

    MFSString ReadLine()
        �ӻ������ж�ȡ��һ���ַ�������ĩ���з��ᱻ�����ڷ��ص��ַ����С�

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
