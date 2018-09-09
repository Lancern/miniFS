#pragma once

#include "../../include/MFSString.h"
#include <vector>



/*

class MFSPath
�ṩ���ļ�ϵͳ·����ͳһ������

    ��̬��Ա������

    static bool MFSPath::IsValidPath(const MFSString & path) noexcept
        ��������·���Ƿ�Ϊһ���Ϸ��� mini-FS ·����
        ��������·����һ�� OS ·�����˺�����ֱ�ӷ��� false��

    static bool MFSPath::IsAbsolutePath(const MFSString & path) noexcept
        ��������·���Ƿ�Ϊһ������·����

    static bool MFSPath::IsOSPath(const MFSString & path) noexcept
        ��������·���Ƿ�Ϊ�ײ����ϵͳ���ļ�·����

    static std::vector<MFSString> MFSPath::GetNames(const MFSString & path)
        ��ȡ������·���еĸ����ļ���Ŀ¼���ơ�
        @exceptions
            MFSInvalidPathException��������·������һ���Ϸ���·����

    static MFSString MFSPath::GetFileName(const MFSString & path)
        ��ȡ������·������ʾ���ļ�����
        @exceptions
            MFSInvalidPathException��������·������һ���Ϸ�·����

    static MFSString MFSPath::GetExtension(const MFSString & path)
        ��ȡ������·������ʾ���ļ�����չ����������·����ʾ���ļ�������չ�����򷵻ص���չ���а���ǰ�� dot �ַ���.����
        ���򷵻�һ���մ���
        @exceptions
            MFSInvalidPathException��������·������һ���Ϸ�·����

    static MFSString MFSPath::GetFileNameWithoutExtension(const MFSString & path)
        ��ȡ������·������ʾ���ļ��Ĳ�������չ�����ļ�����
        @exceptions
            MFSInvalidPathException��������·������һ���Ϸ�·����

    static MFSString MFSPath::GetDirectoryPath(const MFSString & path)
        ��ȡ������·������ʾ���ļ���Ŀ¼�ĸ�Ŀ¼��
        @exceptions
            MFSInvalidPathException��������·������һ���Ϸ�·����

    static MFSString MFSPath::Combine(const MFSString & path1, const MFSString & path2)
        ��ȡ path2 ���� path1 Ϊ��Բο������µľ���·����
        @exceptions
            MFSInvalidPathException��path1 �� path2 ����һ���Ϸ�·����

    static MFSString MFSPath::GetAbsolutePath(const MFSString & path)
        ��ȡ������·���ľ���·����ʽ��
        @exceptions
            MFSInvalidPathException��������·������һ���Ϸ�·����
            MFSDataSpaceNotLoadedException����ǰû���κ��ѹ��ص����ݿռ䡣

    static wchar_t MFSPath::GetPathSeparator() noexcept
        ��ȡϵͳ�����ڷָ�·�����ļ���Ŀ¼�����ַ���

    static std::vector<wchar_t> MFSPath::GetInvalidNameChars() noexcept
        ��ȡϵͳ�ж������Ч�ļ���Ŀ¼���ַ���

*/


class MFSPath
{
public:
    static bool IsValidPath(const MFSString & path) noexcept;
    static bool IsAbsolutePath(const MFSString & path) noexcept;
    static bool IsOSPath(const MFSString & path) noexcept;

    static std::vector<MFSString> GetPathNames(const MFSString & path);

    static MFSString GetFileName(const MFSString & path);
    static MFSString GetExtension(const MFSString & path);
    static MFSString GetFileNameWithoutExtension(const MFSString & path);
    static MFSString GetDirectoryPath(const MFSString & path);

    static MFSString Combine(const MFSString & path1, const MFSString & path2);
    static MFSString GetAbsolutePath(const MFSString & path);

    static wchar_t GetPathSeparator() noexcept;
    static std::vector<wchar_t> GetInvalidNameChars() noexcept;
};
