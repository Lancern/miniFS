#include "../../include/command/TouchCommand.h"

bool TouchCommand::Accept(const MFSString & string) const
{
	if (string == L"touch")
		return true;
	else return false;
}

void TouchCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() != 1)
	{
		point->Log(L"Ö¸ÁîÊäÈëÓÐÎó\n");
		return;
	}
	try
	{
		MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
		MFSFile * file = space->CreateFile(argv[0], false);
	}
	catch(MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
}

TouchCommand::TouchCommand()
{
}