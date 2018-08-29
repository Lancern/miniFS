#include "../../include/command/FormatCommand.h"

bool FormatCommand::Accept(const MFSString & string) const
{
	if (string == L"format" || string == L"fmt")
		return true;
	else return false;
}

void FormatCommand::Action(const std::vector<MFSString> & argv) const
{

}

FormatCommand::FormatCommand()
{
}