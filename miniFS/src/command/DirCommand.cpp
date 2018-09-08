#include "../../include/command/DirCommand.h"

bool DirCommand::Accept(const MFSString & string) const
{
	if (string == L"dr" || string == L"dir" || string == L"ls")
		return true;
	else return false;
}

void DirCommand::Action(const std::vector<MFSString> & argv) const
{
}

DirCommand::DirCommand()
{
}