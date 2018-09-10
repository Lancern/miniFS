#include "../../include/command/FormatCommand.h"

bool FormatCommand::Accept(const MFSString & string) const
{
	if (string == L"format" || string == L"fmt")
		return true;
	else return false;
}

void FormatCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() != 0)
	{
		point->Log(L"Ö¸ÁîÊäÈëÓÐÎó\n");
		return;
	}
	MFSDataSpace * space;
	space = MFSDataSpace::GetActiveDataSpace();
	space->Format();
}

FormatCommand::FormatCommand()
{
}