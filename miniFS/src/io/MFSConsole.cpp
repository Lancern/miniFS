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
    SetConsoleTitleW(string.GetRawString());
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

MFSConsoleColors MFSConsole::GetBackgroundColor() const
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(_hOutput, &bufferInfo);

    WORD attr = (bufferInfo.wAttributes & 0x00F0) >> 4;
    return static_cast<MFSConsoleColors>(attr);
}

MFSConsoleColors MFSConsole::GetForegroundColor() const
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(_hOutput, &bufferInfo);

    WORD attr = bufferInfo.wAttributes & 0x000F;
    return static_cast<MFSConsoleColors>(attr);
}

void MFSConsole::SetBackgroundColor(MFSConsoleColors color)
{
	SetColor(color, GetForegroundColor());
}

void MFSConsole::SetForegroundColor(MFSConsoleColors color)
{
	SetColor(GetBackgroundColor(), color);
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
			if (!buffer.empty() && buffer.back() == '\r')
				buffer.pop_back();
            break;
        }
    }
    return MFSString(buffer.data(), static_cast<uint32_t>(buffer.size()));
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

void MFSConsole::LogLine()
{
    LogChar(L'\n');
}

void MFSConsole::LogLine(const MFSString & string)
{
    Log(string);
    LogLine();
}

void MFSConsole::LogInfoLine(const MFSString & string)
{
    SetColor(MFSConsoleColors::Black, MFSConsoleColors::White);
    LogLine(string);
}

void MFSConsole::LogWarningLine(const MFSString & string)
{
    SetColor(MFSConsoleColors::Black, MFSConsoleColors::Yellow);
    LogLine(string);
}

void MFSConsole::LogErrorLine(const MFSString & string)
{
    SetColor(MFSConsoleColors::Black, MFSConsoleColors::Red);
    LogLine(string);
}

void MFSConsole::SetColor(MFSConsoleColors background, MFSConsoleColors foreground)
{
	SetConsoleTextAttribute(_hOutput, (static_cast<WORD>(background) << 4) | static_cast<WORD>(foreground));
}
