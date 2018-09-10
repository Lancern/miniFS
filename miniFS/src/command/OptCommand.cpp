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
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() != 0)
	{
		point->Log(L"ָ����������\n");
		return;
	}
}

OptCommand::OptCommand()
{
}