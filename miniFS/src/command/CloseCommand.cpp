#include "../../include/command/CloseCommand.h"

bool CloseCommand::Accept(const MFSString & string) const
{
	if (string == L"close")
		return true;
	else return false;
}

void CloseCommand::Action(const std::vector<MFSString> & argv) const
{

}

CloseCommand::CloseCommand()
{
}