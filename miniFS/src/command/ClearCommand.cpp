#include "../../include/command/ClearCommand.h"

bool ClearCommand::Accept(const MFSString & string) const
{
	if (string == L"cls" || string == L"clear")
		return true;
	else return false;
}

void ClearCommand::Action(const std::vector<MFSString> & argv) const
{
	system("cls");
}

void ClearCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->LogLine(L"清空当前屏幕中的显示\n");
}

ClearCommand::ClearCommand()
{
}