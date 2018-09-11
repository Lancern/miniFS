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

void UnmountCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"�������еĸ��Ĳ�ж�ص�ǰ������ݿռ䡣\n\n");
	point->Log(L"ָ���ʽ��\n");
	point->Log(L"unmount\n\n");
	return;
}

UnmountCommand::UnmountCommand()
{
}