#pragma once

#include <cstdint>

/*

class MFSStream
Ϊ�������ṩ������ࡣ

    ��Ա������

    bool MFSStream::CanRead() const
        ������������дʱ������һ�� bool ֵָʾ��ǰ���������Ƿ�֧�ֶ�������

    bool MFSStream::CanWrite() const
        ������������дʱ������һ�� bool ָʾ��ǰ���������Ƿ�֧��д������

    bool MFSStream::CanSeek() const
        ������������дʱ������һ�� bool ָʾ��ǰ���������Ƿ�֧�� seek ������

    bool MFSStream::HasNext() const
        ������������дʱ������һ�� bool ָʾ��ǰ����ָ��֮���Ƿ������ݴ��ڡ�

    uint64_t MFSStream::GetLength() const
        ������������дʱ���������ĳ��ȡ�

    uint64_t MFSStream::GetPosition() const   
        ������������дʱ�����ص�ǰ����ָ�롣

    uint32_t MFSStream::Read(void * lpBuffer, uint32_t count)
        ������������дʱ�������ж�ȡ���ݲ�����ڸ����Ļ������С�
        @param lpBuffer ��Ŷ�ȡ�������ݵĻ������׵�ַ��
        @param dwBufferSize �������ܴ�С��
        @param dwNumberOfBytesToRead Ҫ�����ж�ȡ������ֽ�������
        @return ʵ�ʴ����ж�ȡ���ֽ�������

    uint32_t MFSStream::Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite)
        ������������дʱ��������д�뵽��ǰ���������С�
        @param lpBuffer ���Ҫд������ݵĻ�������
        @param dwNumberOfBytesToWrite Ҫд����ֽ�������
        @returns ʵ��д����ֽ�������

    bool MFSStream::Seek(MFSStreamSeekOrigin origin, INT64 offset)
        ������������дʱ���ƶ���ǰ���������ָ�롣
        @param origin �ƶ�ָ��ʱ����Բο��㡣
        @param offset ָ������ڲο�����ƶ����롣
        @return ����һ�� bool ֵָʾָ���ƶ��Ƿ�ɹ���

    void MFSStream::Flush()
        ������������дʱ�������и���д������豸��

    void MFSStream::Close()
        ������������дʱ���رյ�ǰ���������ͷų��е�������Դ��


enum MFSStreamSeekOrigin
ָʾ�ƶ���ָ��ʱ�Ĳο���λ�á�

    ö�ٳ�Ա��

    Begin
        �ƶ���ָ��ʱ�Ĳο�λ��λ��������ͷ��

    Relative
        �ƶ���ָ��ʱ�Ĳο�λ��λ��������ĵ�ǰ��ָ�봦��

    End
        �ƶ���ָ��ʱ�Ĳο�λ��λ���������β��

*/

enum MFSStreamSeekOrigin
{
    Begin,
    Relative,
    End
};

class MFSStream
{
public:
    virtual bool CanRead() const = 0;
    virtual bool CanWrite() const = 0;
    virtual bool CanSeek() const = 0;

    virtual bool HasNext() const = 0;

    virtual uint64_t GetLength() const = 0;
    virtual uint64_t GetPosition() const = 0;

    virtual uint32_t Read(void * lpBuffer, uint32_t dwBufferSize, uint32_t dwNumberOfBytesToRead) = 0;
    virtual uint32_t Write(const void * lpBuffer, uint32_t dwNumberOfBytesToWrite) = 0;
    virtual bool Seek(MFSStreamSeekOrigin origin, int64_t offset) = 0;

    virtual void Flush() = 0;
    virtual void Close() = 0;
};
