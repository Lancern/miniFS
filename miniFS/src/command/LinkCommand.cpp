#include "../../include/command/LinkCommand.h"

bool LinkCommand::Accept(const MFSString & string) const
{
	if (string == L"link")
		return true;
	else return false;
}

void LinkCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() != 2)
	{
		point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
		return;
	}
}

LinkCommand::LinkCommand()
{
}