#include "../../include/command/FormatCommand.h"

bool FormatCommand::Accept(const MFSString & string) const
{
	if (string == L"format" || string == L"fmt")
		return true;
	else return false;
}

void FormatCommand::Action(const std::vector<MFSString> & argv) const
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
	space->Format();
}

void FormatCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"��ʽ����ǰ������ݿռ�\n\n");
	point->Log(L"ָ���ʽ\n");
	point->Log(L"fmt\n");
	point->Log(L"format\n\n");
	return;
}

FormatCommand::FormatCommand()
{
}