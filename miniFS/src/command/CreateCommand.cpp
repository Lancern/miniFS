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
	if (argv.size() == 2 )
	std::wcout << L"copy file " << argv[0].GetRawString() << L" to file " << argv[1].GetRawString() << std::endl;
}

CreateCommand::CreateCommand()
{
}