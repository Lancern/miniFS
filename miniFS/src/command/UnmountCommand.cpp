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

UnmountCommand::UnmountCommand()
{
}