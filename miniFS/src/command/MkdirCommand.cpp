#include "../../include/command/MkdirCommand.h"

bool MkdirCommand::Accept(const MFSString & string) const
{
	if (string == L"mkdir")
		return true;
	else return false;
}

void MkdirCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSC
	if (argv.size() != 1)
	{
		printf("command is wrong\n");
		return;
	}
	try 
	{
		MFSDataSpace *space;
		space = MFSDataSpace::GetActiveDataSpace();
		space->CreateDirectory(argv[0]);
	}
	catch(MFSException & ex)
	{
		std::wcerr << ex.GetExceptMessage().GetRawString() << std::endl;
	}
}

MkdirCommand::MkdirCommand()
{
}