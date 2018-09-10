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
		point->Log(L"ָ����������\n");
		return;
	}
	if (space != NULL)
	{
		space->Close();
		delete space;
		MFSDataSpace::SetActiveDataSpace(NULL);
		point->Log(L"��ж�ص�ǰ��ռ�\n");
	}
	else
	{
		point->Log(L"��ǰû�л�ռ�\n");
	}
}

UnmountCommand::UnmountCommand()
{
}