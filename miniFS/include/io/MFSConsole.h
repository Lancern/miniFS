#pragma once

#include <Windows.h>
#include "../../../miniFS.Core/include/MFSString.h"


/*

class MFSConsole
为主程序提供标准输入输出。

*/

enum MFSConsoleForegroundColors
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

enum MFSConsoleBackgroundColors
{
    Blue = 0x0010,
    Green = 0x0020,
    Red = 0x0040,
    Cyan = Blue | Green,
    Yellow = Red | Green,
    Purple = Blue | Red,
    White = Blue | Green | Red,
    Intensity = 0x0080
};

class MFSConsole
{
public:
    MFSConsole();
    ~MFSConsole();

    void SetTitle(const MFSString & string);
    MFSString GetTitle() const;

    MFSConsoleBackgroundColors GetBackgroundColor() const;
    MFSConsoleForegroundColors GetForegroundColor() const;

    void SetBackgroundColor(MFSConsoleBackgroundColors color);
    void SetForegroundColor(MFSConsoleForegroundColors color);

    WCHAR ReadChar();
    DWORD ReadKey();
    
    MFSString ReadLine();

    template <typename IntegerT>
    void LogInteger(IntegerT value);

    void LogChar(WCHAR ch);
    void Log(const MFSString & string);

private:
    HANDLE _hInput;
    HANDLE _hOutput;
};

MFSConsole * MFSGetDefaultConsole();

template<typename IntegerT>
inline void MFSConsole::LogInteger(IntegerT value)
{
    if (value < 0)
    {
        value = -value;
        LogChar(L'-');
    }

    while (value)
    {
        IntegerT digit = value % 10;
        LogChar(L'0' + digit);
        value /= 10;
    }
}
