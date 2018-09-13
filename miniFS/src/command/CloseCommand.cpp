#include "../../include/command/CloseCommand.h"

bool CloseCommand::Accept(const MFSString & string) const
{
	if (string == L"close" || string == L"quit")
		return true;
	else return false;
}

void CloseCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace * space = MFSDataSpace::GetActiveDataSpace();
	if (argv.size() != 0)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	if (space != NULL)
	{
		space->Close();
		delete space;
		MFSDataSpace::SetActiveDataSpace(NULL);
	}
	exit(0);
}

void CloseCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"卸载当前活动的数据空间（如果存在），并退出 mini-FS 文件系统。\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"close\n");
}

CloseCommand::CloseCommand()
{
}