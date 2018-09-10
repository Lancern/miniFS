#include "../../include/command/MountCommand.h"

bool MountCommand::Accept(const MFSString & string) const
{
	if (string == L"mount")
		return true;
	else return false;
}

void MountCommand::Action(const std::vector<MFSString> & argv) const
{
	if (argv.size() != 1)
	{
		printf("command is wrong\n");
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
		space->Format();
	}
	catch (MFSException & ex)
	{
		std::wcerr << ex.GetExceptMessage().GetRawString() << std::endl;
	}
}

MountCommand::MountCommand()
{
}