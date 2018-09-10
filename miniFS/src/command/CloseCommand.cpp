#include "../../include/command/CloseCommand.h"

bool CloseCommand::Accept(const MFSString & string) const
{
	if (string == L"close")
		return true;
	else return false;
}

void CloseCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	MFSDataSpace * space = MFSDataSpace::GetActiveDataSpace();
	if (argv.size() != 0)
	{
		point->Log(L"Ö¸ÁîÊäÈëÓÐÎó\n");
		return;
	}
	if (space != NULL)
	{
		space->Close();
		delete space;
		MFSDataSpace::SetActiveDataSpace(NULL);
	}
	exit(0);
}

CloseCommand::CloseCommand()
{
}