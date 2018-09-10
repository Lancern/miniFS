#pragma once
#include "../MFSCommand.h"

class SetHiddenCommand :virtual public MFSCommand
{
public:
	bool Accept(const MFSString & string) const;
	void Action(const std::vector<MFSString> & argv) const;

	SetHiddenCommand();
};