#include "../../include/command/HelpCommand.h"

bool HelpCommand::Accept(const MFSString & string) const
{
	if (string == L"help")
		return true;
	else return false;
}

void HelpCommand::Action(const std::vector<MFSString> & argv) const
{
}

HelpCommand::HelpCommand()
{
}