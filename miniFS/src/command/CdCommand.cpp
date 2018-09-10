#include "../../include/command/CdCommand.h"

bool CdCommand::Accept(const MFSString & string) const
{
	if (string == L"cd")
		return true;
	else return false;
}

void CdCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() > 1)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		if (argv.size() == 0)
			space->SetWorkingDirectory(L"/");
		else
			space->SetWorkingDirectory(argv[0]);
	}
	catch (MFSException &ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");
	}
}

CdCommand::CdCommand()
{
}