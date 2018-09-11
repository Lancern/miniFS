#include "../../include/command/UnmountCommand.h"

bool UnmountCommand::Accept(const MFSString & string) const
{
	if (string == L"unmount")
		return true;
	else return false;
}

void UnmountCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSDataSpace * space = MFSDataSpace::GetActiveDataSpace();
	MFSConsole *point = MFSConsole::GetDefaultConsole();
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
		point->Log(L"已卸载当前活动空间\n");
	}
	else
	{
		point->Log(L"当前没有活动空间\n");
	}
}

void UnmountCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"保存所有的更改并卸载当前活动的数据空间。\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"unmount\n\n");
	return;
}

UnmountCommand::UnmountCommand()
{
}