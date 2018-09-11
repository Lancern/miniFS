#pragma once

#include "../../include/MFSString.h"
#include "../../include/MFSDateTime.h"
#include "../../include/stream/MFSStream.h"

class MFSFSEntry;


/*

class MFSFile
��ʾһ���ļ���

    ��Ա������

    MFSDateTime MFSFile::GetCreationTime() const noexcept
        ��ȡ�ļ��Ĵ���ʱ�䡣���ص�ʱ���Ѿ����ػ���

    MFSDateTime MFSFile::GetLastAccessTime() const noexcept
        ��ȡ�ļ����ϴη���ʱ�䡣���ص�ʱ���Ѿ����ػ���

    MFSDateTime MFSFile::GetLastModificationTime() const noexcept
        ��ȡ�ļ����ϴ��޸�ʱ�䡣���ص�ʱ���Ѿ����ػ���

    bool MFSFile::IsHidden() const noexcept
        ��ȡһ�� bool ֵָʾ�ļ��Ƿ�Ϊ�����ļ���

    void MFSFile::SetHidden(bool isHidden) noexcept
        �����ļ������ر�ǡ�

    uint64_t MFSFile::GetFileSize() const noexcept
        ��ȡ�ļ����ֽڴ�С��

    void MFSFile::SetFileSize(uint64_t size)
        �����ļ����ֽڴ�С�����´�С�����ļ��ĵ�ǰ��С��������Ŀռ佫�ᱻ���Է��䣻���´�СС���ļ��ĵ�ǰ��С�����ļ����ᱻ�ضϣ�
        ���´�С���ļ��ĵ�ǰ��Сһ�£���˺�������ִ���κβ�����
        @exception
            MFSOutOfSpaceException ���ݿռ�ռ䲻�㣬�޷��������Ĳ�����

    std::vector<uint32_t> MFSFile::GetAllBlocksId() const noexcept
        ��ȡ�ļ���ռ�õ��������ݿ���豸���š�

    MFSStream * MFSFile::OpenStream() noexcept
        ���ļ����������ж�д��

*/

class MFSFile
{
public:
    ~MFSFile();

    uint64_t GetFileSize() const noexcept;
    void SetFileSize(uint64_t size);

    std::vector<uint32_t> GetAllBlocksId() const noexcept;

    MFSStream * OpenStream() noexcept;

private:
	MFSFSEntry * _entry;
    MFSFile(MFSFSEntry * entry);

    friend class MFSDataSpace;
};
