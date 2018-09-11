#include "../../include/command/HelpCommand.h"

bool HelpCommand::Accept(const MFSString & string) const
{
	if (string == L"help")
		return true;
	else return false;
}

void HelpCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (argv.size() > 1)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	if (argv.size() == 0)
		Help();
	else
	{
		MFSTest command;
		MFSTestunit *tmp = command.Chead->link;
		while (tmp)
		{
			if (tmp->base->Accept(argv[0]))
			{
				//std::wcout << paragrameter.back().GetRawString();
				tmp->base->Help();
				break;
			}
			tmp = tmp->link;
		}
		if (tmp == NULL) point->Log(L"参数输入有误\n");
	}
}

void HelpCommand::Help() const
{
	return;
}

HelpCommand::HelpCommand()
{
}