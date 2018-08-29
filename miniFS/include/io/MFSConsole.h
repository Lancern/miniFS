#pragma once

#include <Windows.h>
#include "../../../miniFS.Core/include/MFSString.h"


/*

class MFSConsole
为主程序提供标准输入输出。

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

	void SetColor(MFSConsoleColors background, MFSConsoleColors foreground);
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
