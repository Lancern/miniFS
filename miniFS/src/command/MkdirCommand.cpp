#include "../../include/command/MkdirCommand.h"

bool MkdirCommand::Accept(const MFSString & string) const
{
	if (string == L"mkdir")
		return true;
	else return false;
}

void MkdirCommand::Action(const std::vector<MFSString> & argv) const
{

}

MkdirCommand::MkdirCommand()
{
}