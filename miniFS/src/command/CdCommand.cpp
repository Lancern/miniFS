#include "../../include/command/CdCommand.h"

bool CdCommand::Accept(const MFSString & string) const
{
	if (string == L"cd")
		return true;
	else return false;
}

void CdCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() > 1)
	{
		point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
		return;
	}
	exit(0);
}

CdCommand::CdCommand()
{
}