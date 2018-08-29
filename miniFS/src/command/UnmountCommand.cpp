#include "../../include/command/UnmountCommand.h"

bool UnmountCommand::Accept(const MFSString & string) const
{
	if (string == L"unmount")
		return true;
	else return false;
}

void UnmountCommand::Action(const std::vector<MFSString> & argv) const
{

}

UnmountCommand::UnmountCommand()
{
}