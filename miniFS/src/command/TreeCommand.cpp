#include "../../include/command/TreeCommand.h"

bool TreeCommand::Accept(const MFSString & string) const
{
	if (string == L"tree")
		return true;
	else return false;
}

void TreeCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 0)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
}

TreeCommand::TreeCommand()
{
}