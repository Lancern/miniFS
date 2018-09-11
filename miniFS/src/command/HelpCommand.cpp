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
	if (argv.size() != 0)
	{
		point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
		return;
	}

}

HelpCommand::HelpCommand()
{
}