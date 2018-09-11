#pragma once
#include "../MFSCommand.h"

class DelCommand :virtual public MFSCommand
{
public:
	bool Accept(const MFSString & string) const;
	void Action(const std::vector<MFSString> & argv) const;
	void Del(const MFSString & path) const;
	void Help() const;

	DelCommand();
};