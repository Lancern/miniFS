#include "../../include/command/DelCommand.h"

bool DelCommand::Accept(const MFSString & string) const
{
	if (string == L"del" || string == L"dl" ||string == L"rm")
		return true;
	else return false;
}

void DelCommand::Action(const std::vector<MFSString> & argv) const
{

}

DelCommand::DelCommand()
{
}