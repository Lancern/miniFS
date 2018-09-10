#include "../../include/command/LinkCommand.h"

bool LinkCommand::Accept(const MFSString & string) const
{
	if (string == L"link")
		return true;
	else return false;
}

void LinkCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"ָ����������\n");
		return;
	}/*
	try
	{

	}*/
}

LinkCommand::LinkCommand()
{
}