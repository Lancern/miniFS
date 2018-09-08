#pragma once

#include "../../include/MFSString.h"
#include "MFSFile.h"
#include <memory>

class MFSOSFileDevice;
class MFSBlockDevice;
class MFSPartition;


/*

class MFSDataSpace
�ṩ�����ݿռ�Ĳ����������ʵ�������ϵĸ��������ѱ����á�

    ��������

    MFSDataSpace::MFSDataSpace(const MFSString & osFileName)
        �Ӹ����ĵײ��ļ���ʼ�� MFSDataSpace �����ʵ����
        @param osFileName �ײ����ϵͳ�ļ�����
        @exception
            MFSWindowsException �򿪵ײ����ϵͳ�ļ�ʱ���ִ���
            MFSInvalidDeviceException ��Ϊ���豸�ĵײ����ϵͳ�ļ����Ϸ���

    ��Ա������

    MFSString MFSDataSpace::GetWorkingDirectory() const noexcept
        ��ȡ���ݿռ�ĵ�ǰ����Ŀ¼��

    void MFSDataSpace::SetWorkingDirectory(const MFSString & path)
        �������ݿռ�ĵ�ǰ����Ŀ¼��
        @exceptions
            MFSInvalidPathException ������·�����Ϸ���

    uint64_t MFSDataSpace::GetTotalSpaceInBytes() const noexcept
        ��ȡ���ݿռ�����ֽڴ�С��

    uint64_t MFSDataSpace::GetFreeSpaceInBytes() const noexcept
        ��ȡ���ݿռ���ܿ��ô�С��

    bool MFSDataSpace::IsFormatted() const noexcept
        ��ȡһ�� bool ֵָʾ��ǰ�����ݿռ��Ƿ��Ѿ�������ʽ����

    void MFSDataSpace::Format() noexcept
        ��ʽ����ǰ�����ݿռ䡣

    bool MFSDataSpace::Exist(const MFSString & path)
        ��������·���Ƿ���ڡ�
        @exceptions
            MFSInvalidPathException��������·������һ���Ϸ�·����

    MFSFile * MFSDataSpace::OpenFile(const MFSString & path, bool createIfNotExist)
        ��һ���ļ���
        @param path �ļ���·����
        @param createIfNotExist ���ò���Ϊ true ʱ�����ļ������ڽ������ļ��������׳� MFSFileNotFoundException �쳣��
        @exceptions
            MFSInvalidPathException ������·�����Ϸ���
            MFSDirectoryNotFoundException ·������һ������Ŀ¼�����ڡ�
            MFSFileNotFoundException �ļ��������� createIfNotExist ����Ϊ false��

    MFSFile * MFSDataSpace::CreateFile(const MFSString & path, bool openIfExist)
        ����һ���ļ���
        @param path �ļ���·����
        @param openIfExist ���ò���Ϊ true ʱ�����ļ��Ѿ����ڽ�ֱ�Ӵ��ļ��������׳� MFSFileAlreadyExistException �쳣��
        @exceptions
            MFSInvalidPathException ������·�����Ϸ���
            MFSDirectoryNotFoundException ·������һ������Ŀ¼�����ڡ�
            MFSFileAlreadyExistException �ļ��Ѿ������� openIfExist ����Ϊ false��
            MFSOutOfSpaceException ���ݿռ�ռ䲻�㣬�޷����Ҫ��Ĳ�����

    void MFSDataSpace::CreateDirectory(const MFSString & path)
        ����һ��Ŀ¼�������в����ڵ�ֱ�ӻ��Ӹ�Ŀ¼��
        @param path Ŀ¼��·����
        @exceptions
            MFSInvalidPathException ������·�����Ϸ���
            MFSFileAlreadyExist ·���ϴ���һ������ͬ���ļ���
            MFSOutOfSpaceException ���ݿռ�ռ䲻�㣬�޷����Ҫ��Ĳ�����
            
    void MFSDataSpace::CreateLink(const MFSString & target, const MFSString & link)
        ����һ��Ӳ���ӡ�
        @param target Ӳ���ӵ�Ŀ�ꡣ
        @param link Ӳ���ӵ�Դ��
        @exceptions
            MFSInvalidPathException Ŀ��·�� target �� Դ·��link ���Ϸ���
            MFSDirectoryNotFoundException ·������һ������Ŀ¼�����ڡ�
            MFSFileAlreadyExist ·���ϴ���һ������ͬ���ļ���
            MFSOutOfSpaceException ���ݿռ�ռ䲻�㣬�޷����Ҫ��Ĳ�����

    void MFSDataSpace::Delete(const MFSString & path)
        ɾ��һ��Ŀ¼���ļ���Ӳ���ӡ�
        @param path Ҫɾ����Ŀ¼���ļ���Ӳ���ӵ����ơ�
        @exceptions
            MFSInvalidPathException ·�����Ϸ���
            MFSDirectoryNotFoundException ·������һ������Ŀ¼�����ڡ�
            MFSFileNotFoundException Ҫɾ����Ŀ¼���ļ���Ӳ���Ӳ����ڡ�

    void MFSDataSpace::Copy(const MFSString & source, const MFSString & destination)
        ����һ��Ŀ¼���ļ���
        @param source Դ·����
        @param destination Ŀ��·����
        @exception
            MFSInvalidPathException Դ·�� source �� Ŀ��·�� destination ���Ϸ���
            MFSDirectoryNotFoundException Դ·����Ŀ��·���ϵ�һ������Ŀ¼�����ڡ�
            MFSFileNotFoundException Ҫ���Ƶ�Դ�ļ���Ŀ���ļ������ڡ�
            MFSOutOfSpaceException ���ݿռ�ռ䲻�㣬�޷����Ҫ��Ĳ�����

    void MFSDataSpace::Move(const MFSString & source, const MFSString & destination)
        �ƶ�һ��Ŀ¼���ļ���
        @param source Դ·����
        @param destination Ŀ��·����
        @exception
            MFSInvalidPathException Դ·�� source �� Ŀ��·�� destination ���Ϸ���
            MFSDirectoryNotFoundException Դ·����Ŀ��·���ϵ�һ������Ŀ¼�����ڡ�
            MFSFileNotFoundException Ҫ�ƶ���Դ�ļ���Ŀ���ļ������ڡ�

    std::vector<MFSString> MFSDataSpace::GetDirectories(const MFSString & directory)
        ��ȡĳ��Ŀ¼�µ�������Ŀ¼���ơ�
        @param directory Ҫ��ȡ������Ŀ¼��Ŀ¼·����
        @exception
            MFSInvalidPathException ������·�����Ϸ���
            MFSDirectoryNotFoundException ������·������һ������Ŀ¼�����ڡ�

    void MFSDataSpace::GetFiles(const MFSString & directory)
        ��ȡĳ��Ŀ¼�µ��������ļ����ơ�
        @param directory Ҫ��ȡ�������ļ���Ŀ¼·����
        @exception
            MFSInvalidPathException ������·�����Ϸ���
            MFSDirectoryNotFoundException ������·������һ������Ŀ¼�����ڡ�

    void MFSDataSpace::Close() noexcept
        �رյ�ǰ�����ݿռ�����ͷ���ռ�õ������ⲿ��Դ��


    ��̬��Ա������

    static MFSDataSpace * MFSDataSpace::CreateDataSpace(const MFSString & filename, uint64_t size)
        ����һ�����ݿռ䡣
        @param filename ���ݿռ�����Ӧ�ĵײ����ϵͳ�ļ�����
        @param size ���ݿռ�����ֽڴ�С���ò�����Ӧ��С�� 134,217,728���� 128 MB��ͬʱ��Ӧ�ô��� 4,294,967,296���� 4GB��
                    �ò����ں����ڲ����ᱻ���϶��뵽 4KB ����������
        @exceptions
            std::invalid_argument size ��С�����
            MFSWindowsException �������ϵͳ���������г��ִ���

    static MFSDataSpace * MFSDataSpace::GetActiveDataSpace() noexcept
        ��ȡӦ�ó����е�ǰ������ݿռ䡣����ǰӦ�ó�����û�л�����ݿռ䣬���� nullptr��

    static void MFSDataSpace::SetActiveDataSpace(MFSDataSpace * dataSpace) noexcept
        ����Ӧ�ó����еĻ�����ݿռ䡣�˲�����ת�ô����ָ�������Ȩ��

*/

#ifdef CreateFile
// Undef Windows-defined macro CreateFile for it conflicts with the MFSDataSpace::CreateFile.
#undef CreateFile
#endif

#ifdef CreateDirectory
// Undef Windows-defined macro CreateFile for it conflicts with the MFSDataSpace::CreateDirectory.
#undef CreateDirectory
#endif

class MFSDataSpace
{
public:
    MFSDataSpace(const MFSString & osFileName);

    MFSDataSpace(const MFSDataSpace &) = delete;
    MFSDataSpace(MFSDataSpace && another);

    MFSDataSpace & operator = (const MFSDataSpace &) = delete;
    MFSDataSpace & operator = (MFSDataSpace && another);

    MFSString GetWorkingDirectory() const noexcept;
    void SetWorkingDirectory(const MFSString & path);

    uint64_t GetTotalSpaceInBytes() const noexcept;
    uint64_t GetFreeSpaceInBytes() const noexcept;

    bool IsFormatted() const noexcept;
    
    void Format() noexcept;

    bool Exist(const MFSString & path);
    MFSFile * OpenFile(const MFSString & path, bool createIfNotExist);
    MFSFile * CreateFile(const MFSString & path, bool openIfExist);
    void CreateDirectory(const MFSString & path);
    void CreateLink(const MFSString & target, const MFSString & link);
    void Delete(const MFSString & path);

    void Copy(const MFSString & source, const MFSString & destination);
    void Move(const MFSString & source, const MFSString & destination);

    std::vector<MFSString> GetDirectories(const MFSString & directory);
    std::vector<MFSString> GetFiles(const MFSString & directory);

    void Close() noexcept;

    static MFSDataSpace * CreateDataSpace(const MFSString & filename, uint64_t size);
    
    static MFSDataSpace * GetActiveDataSpace() noexcept;
    static void SetActiveDataSpace(MFSDataSpace * dataSpace) noexcept;

private:
    MFSString _workingDirectory;

    HANDLE _hFile;
    std::shared_ptr<MFSOSFileDevice> _fileDevice;
    std::shared_ptr<MFSBlockDevice> _blockDevice;
    // NOTICE: MFSPartition is an incomplete type and yet we should use std::shared_ptr as below shows.
    std::shared_ptr<MFSPartition> _partition;
};
