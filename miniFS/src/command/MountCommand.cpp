#include "../../include/command/MountCommand.h"

bool MountCommand::Accept(const MFSString & string) const
{
	if (string == L"mount")
		return true;
	else return false;
}

void MountCommand::Action(const std::vector<MFSString> & argv) const
{

}

MountCommand::MountCommand()
{
}