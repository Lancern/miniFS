#pragma once

#include <Windows.h>
#include "../../../miniFS.Core/include/MFSString.h"


/*

class MFSConsole
为主程序提供标准输入输出。

    成员函数：

    void MFSConsole::SetTitle(const MFSString & string)
        设置命令行窗口的标题。

    MFSString MFSConsole::GetTitle() const
        获取命令行窗口的标题。

    MFSConsoleColors MFSConsole::GetBackgroundColor() const
        获取命令行窗口的背景颜色。

    MFSConsoleColors MFSConsole::GetForegroundColor() const
        获取命令行窗口的文字颜色。

    void MFSConsole::SetBackgroundColor(MFSConsoleColors color)
        设置命令行窗口的背景颜色。

    void MFSConsole::SetForegroundColor(MFSConsoleColors color)
        设置命令行窗口的文字颜色。

    void MFSConsole::SetColor(MFSConsoleColors background, MFSConsoleColors foreground)
        设置命令行窗口的背景颜色和文字颜色。

    WCHAR MFSConsole::ReadChar()
        从输入流读取命令行的下一个字符。

    DWORD MFSConsole::ReadKey()
        从输入流读取用户按下的键盘按键。
        @return 返回用户按下的按键的按键码。

    MFSString MFSConsole::ReadLine()
        从输入流读取一行输入。

    void MFSConsole::LogChar(WCHAR ch)
        向输出流写入一个字符。

    void MFSConsole::Log(const MFSString & string)
        向输出流写入一个字符串。

    void MFSConsole::LogLine()
        向输出流写入一个换行符。

    void MFSConsole::LogLine(const MFSString & string)
        向输出流写入一个字符串，然后写入一个换行符。

    void MFSConsole::LogInfoLine(const MFSString & string)
        以 Info 格式向输出流写入一个字符串，然后写入一个换行符。

    void MFSConsole::LogWarningLine(const MFSString & string)
        以 Warning 格式向输出流写入一个字符串，然后写入一个换行符。

    void MFSConsole::LogErrorLine(const MFSString & string)
        以 Error 格式向输出流写入一个字符串，然后写入一个换行符。


MFSConsole * MFSGetDefaultConsole()
    获取当前程序的默认 MFSConsole 对象。

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
