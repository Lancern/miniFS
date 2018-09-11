#include "../../include/command/OptCommand.h"

bool OptCommand::Accept(const MFSString & string) const
{
	if (string == L"opt")
		return true;
	else return false;
}

void OptCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 0)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
}

void OptCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"执行分区碎片整理。\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"opt\n\n");
	return;
}

OptCommand::OptCommand()
{
}