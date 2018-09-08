#pragma once

#include "../../include/MFSString.h"
#include "MFSFile.h"
#include <memory>

class MFSPartition;


/*

class MFSDataSpace
�ṩ�����ݿռ�Ĳ�����

    ��������

    MFSDataSpace::MFSDataSpace(const MFSString & osFileName)
        �Ӹ����ĵײ��ļ���ʼ�� MFSDataSpace �����ʵ����
        @param osFileName �ײ����ϵͳ�ļ�����

    ��Ա������

    MFSString MFSDataSpace::GetWorkingDirectory() const
        ��ȡ���ݿռ�ĵ�ǰ����Ŀ¼��

    void MFSDataSpace::SetWorkingDirectory() const
        �������ݿռ�ĵ�ǰ����Ŀ¼��

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


    ��̬��Ա������

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

    MFSString GetWorkingDirectory() const noexcept;
    void SetWorkingDirectory(const MFSString & path) noexcept;

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
    
    static MFSDataSpace * GetActiveDataSpace() noexcept;
    static void SetActiveDataSpace(MFSDataSpace * dataSpace) noexcept;

private:
    MFSString _workingDirectory;
    // NOTICE: MFSPartition is an incomplete type and yet we should use std::shared_ptr as below shows.
    std::shared_ptr<MFSPartition> _partition;
};
