#include "../../include/command/MapCommand.h"

bool MapCommand::Accept(const MFSString & string) const
{
	if (string == L"map")
		return true;
	else return false;
}

void MapCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() != 1)
	{
		point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
		return;
	}
}

MapCommand::MapCommand()
{
}