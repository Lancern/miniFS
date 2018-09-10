#include "../../include/command/OptCommand.h"

bool OptCommand::Accept(const MFSString & string) const
{
	if (string == L"opt")
		return true;
	else return false;
}

void OptCommand::Action(const std::vector<MFSString> & argv) const
{

}

OptCommand::OptCommand()
{
}