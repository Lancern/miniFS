#include "../../include/command/FormatCommand.h"

bool FormatCommand::Accept(const MFSString & string) const
{
	if (string == L"format" || string == L"fmt")
		return true;
	else return false;
}

void FormatCommand::Action(const std::vector<MFSString> & argv) const
{
	if (argv.size() != 0)
	{
		printf("command is wrong\n");
		return;
	}
	MFSDataSpace * space;
	space = MFSDataSpace::GetActiveDataSpace();
	space->Format();
}

FormatCommand::FormatCommand()
{
}