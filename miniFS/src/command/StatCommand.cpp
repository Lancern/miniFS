#include "../../include/command/StatCommand.h"

bool StatCommand::Accept(const MFSString & string) const
{
	if (string == L"stat" || string == L"attr")
		return true;
	else return false;
}

void StatCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() != 1)
	{
		point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
		return;
	}
}

StatCommand::StatCommand()
{
}