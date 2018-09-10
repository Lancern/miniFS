#include "../../include/command/CloseCommand.h"

bool CloseCommand::Accept(const MFSString & string) const
{
	if (string == L"close")
		return true;
	else return false;
}

void CloseCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() != 0)
	{
		point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
		return;
	}
	exit(0);
}

CloseCommand::CloseCommand()
{
}