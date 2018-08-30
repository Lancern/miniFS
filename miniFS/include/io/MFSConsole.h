#pragma once

#include <Windows.h>
#include "../../../miniFS.Core/include/MFSString.h"


/*

class MFSConsole
Ϊ�������ṩ��׼���������

    ��Ա������

    void MFSConsole::SetTitle(const MFSString & string)
        ���������д��ڵı��⡣

    MFSString MFSConsole::GetTitle() const
        ��ȡ�����д��ڵı��⡣

    MFSConsoleColors MFSConsole::GetBackgroundColor() const
        ��ȡ�����д��ڵı�����ɫ��

    MFSConsoleColors MFSConsole::GetForegroundColor() const
        ��ȡ�����д��ڵ�������ɫ��

    void MFSConsole::SetBackgroundColor(MFSConsoleColors color)
        ���������д��ڵı�����ɫ��

    void MFSConsole::SetForegroundColor(MFSConsoleColors color)
        ���������д��ڵ�������ɫ��

    void MFSConsole::SetColor(MFSConsoleColors background, MFSConsoleColors foreground)
        ���������д��ڵı�����ɫ��������ɫ��

    WCHAR MFSConsole::ReadChar()
        ����������ȡ�����е���һ���ַ���

    DWORD MFSConsole::ReadKey()
        ����������ȡ�û����µļ��̰�����
        @return �����û����µİ����İ����롣

    MFSString MFSConsole::ReadLine()
        ����������ȡһ�����롣

    void MFSConsole::LogChar(WCHAR ch)
        �������д��һ���ַ���

    void MFSConsole::Log(const MFSString & string)
        �������д��һ���ַ�����

    void MFSConsole::LogLine()
        �������д��һ�����з���

    void MFSConsole::LogLine(const MFSString & string)
        �������д��һ���ַ�����Ȼ��д��һ�����з���

    void MFSConsole::LogInfoLine(const MFSString & string)
        �� Info ��ʽ�������д��һ���ַ�����Ȼ��д��һ�����з���

    void MFSConsole::LogWarningLine(const MFSString & string)
        �� Warning ��ʽ�������д��һ���ַ�����Ȼ��д��һ�����з���

    void MFSConsole::LogErrorLine(const MFSString & string)
        �� Error ��ʽ�������д��һ���ַ�����Ȼ��д��һ�����з���


MFSConsole * MFSGetDefaultConsole()
    ��ȡ��ǰ�����Ĭ�� MFSConsole ����

*/

enum MFSConsoleColors
{
    Black = 0x0000,
    Blue = 0x0001,
    Green = 0x0002,
    Red = 0x0004,
    Cyan = Blue | Green,
    Yellow = Red | Green,
    Purple = Blue | Red,
    White = Blue | Green | Red,
    Intensity = 0x0008
};

class MFSConsole
{
public:
    MFSConsole();
    ~MFSConsole();

    void SetTitle(const MFSString & string);
    MFSString GetTitle() const;

    MFSConsoleColors GetBackgroundColor() const;
    MFSConsoleColors GetForegroundColor() const;
    void SetBackgroundColor(MFSConsoleColors color);
    void SetForegroundColor(MFSConsoleColors color);
    void SetColor(MFSConsoleColors background, MFSConsoleColors foreground);

    WCHAR ReadChar();
    DWORD ReadKey();
    MFSString ReadLine();

    void LogChar(WCHAR ch);
    void Log(const MFSString & string);
    void LogLine();
    void LogLine(const MFSString & string);

    void LogInfoLine(const MFSString & string);
    void LogWarningLine(const MFSString & string);
    void LogErrorLine(const MFSString & string);

private:
    HANDLE _hInput;
    HANDLE _hOutput;
};

MFSConsole * MFSGetDefaultConsole();
