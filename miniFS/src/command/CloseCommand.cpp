#include "../../include/command/CloseCommand.h"

bool CloseCommand::Accept(const MFSString & string) const
{
	if (string == L"close")
		return true;
	else return false;
}

void CloseCommand::Action(const std::vector<MFSString> & argv) const
{
	if (argv.size() != 0)
	{
		printf("command is wrong\n");
		return;
	}
	exit(0);
}

CloseCommand::CloseCommand()
{
}