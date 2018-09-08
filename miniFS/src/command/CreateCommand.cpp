#include "../../include/command/CreateCommand.h"

bool CreateCommand::Accept(const MFSString & string) const
{
	if (string == L"create")
		return true;
	else return false;
}

void CreateCommand::Action(const std::vector<MFSString> & argv) const
{
	if (argv.size() < 0 || argv.size() > 2)
	{
		printf("command is wrong\n");
		return;
	}
	try
	{
		MFSDataSpace * space = MFSDataSpace::CreateDataSpace(argv[0], 1024 * 1024 * 1024);
		space->Close();
	}
	catch (const std::exception&)
	{

	}
}

CreateCommand::CreateCommand()
{
}