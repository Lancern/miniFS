#include "../../include/command/FormatCommand.h"

bool FormatCommand::Accept(const MFSString & string) const
{
	if (string == L"format" || string == L"fmt")
		return true;
	else return false;
}

void FormatCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
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
	space->Format();
}

FormatCommand::FormatCommand()
{
}