#include "../../include/command/CopyCommand.h"

bool CopyCommand::Accept(const MFSString & string) const
{
	if (string == L"copy" || string == L"cp")
		return true;
	else return false;
}

void CopyCommand::Action(const std::vector<MFSString> & argv) const
{
	if (argv.size() != 2)
	{
		printf("command is wrong\n");
		return;
	}
	/*std::wcout << L"copy file " << argv[0].GetRawString() << L" to file " << argv[1].GetRawString()<<std::endl;*/
	MFSPath *path = new MFSPath();
	MFSString from = argv[0];
	MFSString to = argv[1];
	try
	{
		if (path->IsValidPath(from)) {

		}
	}
	catch (const std::exception&)
	{

	}
}

CopyCommand::CopyCommand()
{
}