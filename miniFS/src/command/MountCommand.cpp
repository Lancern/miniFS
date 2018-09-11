#include "../../include/command/MountCommand.h"

bool MountCommand::Accept(const MFSString & string) const
{
	if (string == L"mount")
		return true;
	else return false;
}

void MountCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (argv.size() != 1)
	{
		point->Log(L"Ö¸ÁîÊäÈëÓÐÎó\n");
		return;
	}
	try
	{
		MFSDataSpace * space;
		space = MFSDataSpace::GetActiveDataSpace();
		if (space != NULL)
		{
			space->Close();
			delete space;
		}
		space = new MFSDataSpace(argv[0]);
		MFSDataSpace::SetActiveDataSpace(space);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");;
	}
}

void MountCommand::Help() const
{
	return;
}

MountCommand::MountCommand()
{
}