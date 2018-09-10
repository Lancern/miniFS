#include "../../include/command/CopyCommand.h"

bool CopyCommand::Accept(const MFSString & string) const
{
	if (string == L"copy" || string == L"cp")
		return true;
	else return false;
}

void CopyCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"��ǰδ���ؿռ�");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	
}

CopyCommand::CopyCommand()
{
}