#include "../../include/command/MapCommand.h"

bool MapCommand::Accept(const MFSString & string) const
{
	if (string == L"map")
		return true;
	else return false;
}

void MapCommand::Action(const std::vector<MFSString> & argv) const
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
}

MapCommand::MapCommand()
{
}