#include "../include/MFSCommand.h"

MFSCommand::MFSCommand()
{
}

bool MFSCommand::Accept(const MFSString & string) const
{
	if (string == NULL)
		return false;
	else return true;
}

void MFSCommand::Action() const
{
}