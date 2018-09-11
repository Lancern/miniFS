#pragma once

#include "MFSBlockDevice.h"
#include <cstdint>

/*

class MFSSparseDevice
��ʾ֧��ϡ�����ݿ�������豸��

    ��Ա������

    virtual bool MFSSparseDevice::ZeroRegion(uint64_t startOffset, uint64_t length) = 0
        ������������дʱ�����豸�е�ĳ���������㡣
        @param startOffset Ҫ�������������ֽ�ƫ������
        @param length Ҫ��������򳤶ȡ�
        @return ����һ�� bool ֵ��ʾ�����Ƿ�ɹ���

    virtual bool MFSSparseDevice::ZeroAll() = 0
        ������������дʱ���������豸���㡣
        @return ����һ�� bool ֵ��ʾ�����Ƿ�ɹ���

    virtual bool MFSSparseDevice::SupportSparse() const = 0
        ������������дʱ������һ�� bool ֵ��������豸�Ƿ�֧��ϡ�����ݲ�����

*/


class MFSSparseDevice
    : public virtual MFSRawDevice
{
public:
    virtual bool ZeroRegion(uint64_t startOffset, uint64_t length) = 0;
    virtual bool ZeroAll() = 0;
    virtual bool SupportSparse() const = 0;
};
