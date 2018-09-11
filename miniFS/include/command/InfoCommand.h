#pragma once
#include "../MFSCommand.h"

class InfoCommand :virtual public MFSCommand
{
public:
	bool Accept(const MFSString & string) const;
	void Action(const std::vector<MFSString> & argv) const;
	void Help() const;

	InfoCommand();
};