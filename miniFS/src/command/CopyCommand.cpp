#include "../../include/command/CopyCommand.h"

bool CopyCommand::Accept(const MFSString & string) const
{
	if (string == L"copy" || string == L"cp")
		return true;
	else return false;
}

void CopyCommand::Action(const std::vector<MFSString> & argv) const
{
}

CopyCommand::CopyCommand()
{
}