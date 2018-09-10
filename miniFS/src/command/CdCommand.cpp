#include "../../include/command/CdCommand.h"

bool CdCommand::Accept(const MFSString & string) const
{
	if (string == L"cd")
		return true;
	else return false;
}

void CdCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() > 1)
	{
		point->Log(L"Ö¸ÁîÊäÈëÓÐÎó\n");
		return;
	}
	try
	{
		MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
		if (argv.size() == 0)
			space->SetWorkingDirectory(L"/");
		else
			space->SetWorkingDirectory(argv[0]);
	}
	catch (MFSException &ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");
	}
}

CdCommand::CdCommand()
{
}