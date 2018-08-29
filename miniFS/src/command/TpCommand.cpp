#include "../../include/command/TpCommand.h"

bool TpCommand::Accept(const MFSString & string) const
{
	if (string == L"tp" || string == L"cat")
		return true;
	else return false;
}

void TpCommand::Action(const std::vector<MFSString> & argv) const
{

}

TpCommand::TpCommand()
{
}