#include "../../include/command/InfoCommand.h"

bool InfoCommand::Accept(const MFSString & string) const
{
	if (string == L"info")
		return true;
	else return false;
}

void InfoCommand::Action(const std::vector<MFSString> & argv) const
{
	if (argv.size() != 0)
	{
		printf("command is wrong\n");
		return;
	}
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (!space)
	{
		printf("并没有挂载任何空间\n");
	}
	else {

	}
}

InfoCommand::InfoCommand()
{
}