#include "../../include/command/FormatCommand.h"

bool FormatCommand::Accept(const MFSString & string) const
{
	if (string == L"format" || string == L"fmt")
		return true;
	else return false;
}

void FormatCommand::Action(const std::vector<MFSString> & argv) const
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
	space->Format();
}

void FormatCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"格式化当前活动的数据空间\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"fmt\n");
	point->Log(L"format\n\n");
	return;
}

FormatCommand::FormatCommand()
{
}