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
		point->Log(L"ָ����������\n");
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
	point->Log(L"ж�ص�ǰ������ݿռ䣨������ڣ������˳� mini-FS �ļ�ϵͳ��\n\n");
	point->Log(L"ָ���ʽ\n");
	point->Log(L"close\n");
}

CloseCommand::CloseCommand()
{
}