#include "../../include/command/StatCommand.h"

bool StatCommand::Accept(const MFSString & string) const
{
	if (string == L"stat" || string == L"attr")
		return true;
	else return false;
}

void StatCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() != 1)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		MFSFile *file = space->OpenFile(argv[0], false);
	}
	catch (MFSException & ex)
	{
		point->LogLine(ex.GetExceptMessage());
	}
}

StatCommand::StatCommand()
{
}