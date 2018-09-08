#include "../../include/command/MoveCommand.h"

bool MoveCommand::Accept(const MFSString & string) const
{
	if (string == L"move" || string == L"mv")
		return true;
	else return false;
}

void MoveCommand::Action(const std::vector<MFSString> & argv) const
{
}

MoveCommand::MoveCommand()
{
}