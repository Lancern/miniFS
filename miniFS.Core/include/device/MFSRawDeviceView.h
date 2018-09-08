#pragma once

#include <cstdint>


/*

class MFSRawDeviceView
Ϊ MFSRawDevice �ṩ������ͼ���ࡣ

    ��Ա������
    
    bool MFSRawDeviceView::CanWrite() const
        ���������б���дʱ������һ�� bool ֵ��ʾ�Ƿ������ǰ�Ĳ�����ͼд�����ݡ�

    uint32_t MFSRawDeviceView::GetSize() const
        ���������б���дʱ�����ص�ǰ��ͼ���ֽڴ�С��

    uint32_t MFSRawDeviceView::Read(void * lpBuffer, uint32_t offset, uint32_t length)
        ���������б���дʱ���ӵ�ǰ��ͼ�ж�ȡ���ݲ������ڸ����Ļ������С�
        @param lpBuffer Ҫ��Ŷ�ȡ�����ݵĻ������׵�ַָ�롣
        @param offset �ӵ�ǰ��ͼ�п�ʼ��ȡ�����ݵ�ƫ������
        @param length Ҫ�ӵ�ǰ��ͼ�ж�ȡ�����ݵ��ֽ�������
        @returns �ӵ�ǰ��ͼ�гɹ���ȡ�����ֽ�������

    uint32_t MFSDeviceVew::Write(uint32_t offset, uint32_t length, const void * lpBuffer)
        ���������б���дʱ����ǰ��ͼ��д�����ݡ�
        @param offset Ҫд��������ڵ�ǰ��ͼ�е�ƫ������
        @param length Ҫд������ݵ��ֽڳ��ȡ�
        @param lpBuffer ���Ҫд������ݵĻ������׵�ַָ�롣

    void MFSRawDeviceView::Flush()
        ���������б���дʱ�������и���д������豸��

    void MFSRawDeviceView::Close()
        ���������б���дʱ�������и���д������豸���رյ�ǰ����ͼ����

*/

class MFSRawDeviceView
{
public:
    virtual bool CanWrite() const = 0;
    virtual uint32_t GetSize() const = 0;

    virtual uint32_t Read(void * lpBuffer, uint32_t offset, uint32_t length) = 0;
    virtual uint32_t Write(uint32_t offset, uint32_t length, const void * lpBuffer) = 0;

    virtual void Flush() = 0;
    virtual void Close() = 0;
};

