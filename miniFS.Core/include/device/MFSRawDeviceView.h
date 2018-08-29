#pragma once

#include <Windows.h>

/*

class MFSRawDeviceView
Ϊ MFSRawDevice �ṩ������ͼ���ࡣ

    ��Ա������
    
    bool MFSRawDeviceView::CanWrite() const
        ���������б���дʱ������һ�� bool ֵ��ʾ�Ƿ������ǰ�Ĳ�����ͼд�����ݡ�

    DWORD MFSRawDeviceView::GetSize() const
        ���������б���дʱ�����ص�ǰ��ͼ���ֽڴ�С��

    DWORD MFSRawDeviceView::Read(LPVOID lpBuffer, DWORD offset, DWORD length)
        ���������б���дʱ���ӵ�ǰ��ͼ�ж�ȡ���ݲ������ڸ����Ļ������С�
        @param lpBuffer Ҫ��Ŷ�ȡ�����ݵĻ������׵�ַָ�롣
        @param offset �ӵ�ǰ��ͼ�п�ʼ��ȡ�����ݵ�ƫ������
        @param length Ҫ�ӵ�ǰ��ͼ�ж�ȡ�����ݵ��ֽ�������
        @returns �ӵ�ǰ��ͼ�гɹ���ȡ�����ֽ�������

    DWORD MFSDeviceVew::Write(DWORD offset, DWORD length, LPCVOID lpBuffer)
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
    virtual ~MFSRawDeviceView();

    virtual bool CanWrite() const = 0;
    virtual DWORD GetSize() const = 0;

    virtual DWORD Read(LPVOID lpBuffer, DWORD offset, DWORD length) = 0;
    virtual DWORD Write(DWORD offset, DWORD length, LPCVOID lpBuffer) = 0;

    virtual void Flush() = 0;
    virtual void Close() = 0;

protected:
    MFSRawDeviceView();
};

