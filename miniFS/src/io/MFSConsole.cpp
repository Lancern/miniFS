#include "../../include/io/MFSConsole.h"


MFSConsole * _defaultConsole;

MFSConsole * MFSGetDefaultConsole()
{
    if (!_defaultConsole)
        _defaultConsole = new MFSConsole();
    return _defaultConsole;
}

MFSConsole::MFSConsole()
{
    _hInput = GetStdHandle(STD_INPUT_HANDLE);
    _hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
}

MFSConsole::~MFSConsole()
{
    CloseHandle(_hInput);
    CloseHandle(_hOutput);
}

void MFSConsole::SetTitle(const MFSString & string)
{
    SetConsoleTitle(string.GetRawString());
}

MFSString MFSConsole::GetTitle() const
{
    WCHAR tmp;
    DWORD dwTitleLen = GetConsoleTitleW(&tmp, 1);

    WCHAR * buffer = new WCHAR[dwTitleLen + 1];
    GetConsoleTitleW(buffer, dwTitleLen + 1);

    MFSString ret(buffer);
    delete[] buffer;

    return ret;
}

MFSConsoleBackgroundColors MFSConsole::GetBackgroundColor() const
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(_hOutput, &bufferInfo);

    DWORD attr = bufferInfo.wAttributes & 0x000F;
    return static_cast<MFSConsoleBackgroundColors>(attr);
}

MFSConsoleForegroundColors MFSConsole::GetForegroundColor() const
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(_hOutput, &bufferInfo);

    DWORD attr = bufferInfo.wAttributes & 0x00F0;
    return static_cast<MFSConsoleForegroundColors>(attr);
}

void MFSConsole::SetBackgroundColor(MFSConsoleBackgroundColors color)
{
    SetConsoleTextAttribute(_hOutput, static_cast<WORD>(color));
}

void MFSConsole::SetForegroundColor(MFSConsoleForegroundColors color)
{
    SetConsoleTextAttribute(_hOutput, static_cast<WORD>(color));
}

WCHAR MFSConsole::ReadChar()
{
    WCHAR buffer;
    DWORD read;
    ReadConsole(_hInput, &buffer, 1, &read, NULL);
    return buffer;
}

DWORD MFSConsole::ReadKey()
{
    while (true)
    {
        INPUT_RECORD record;
        DWORD read;
        ReadConsoleInput(_hInput, &record, 1, &read);
        if (record.EventType == KEY_EVENT)
            return record.Event.KeyEvent.wVirtualKeyCode;
    }
    return 0;
}

MFSString MFSConsole::ReadLine()
{
    std::vector<WCHAR> buffer;
    while (true)
    {
        buffer.push_back(ReadChar());
        if (buffer.back() == L'\n')
        {
            buffer.pop_back();
            break;
        }
    }
    return MFSString(buffer.data(), buffer.size());
}

void MFSConsole::LogChar(WCHAR ch)
{
    DWORD write;
    WriteConsoleW(_hOutput, &ch, 1, &write, NULL);
}

void MFSConsole::Log(const MFSString & string)
{
    for (WCHAR ch : string)
        LogChar(ch);
}
