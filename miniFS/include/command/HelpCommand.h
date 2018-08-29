#pragma once
#include "../MFSCommand.h"

class HelpCommand : public MFSCommand
{
public:
	bool Accept(const MFSString & string) const;
	void Action(const std::vector<MFSString> & argv) const;

protected:
	HelpCommand();
};