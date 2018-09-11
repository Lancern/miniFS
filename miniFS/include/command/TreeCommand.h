#pragma once
#include "../MFSCommand.h"

class TreeCommand :virtual public MFSCommand
{
public:
	bool Accept(const MFSString & string) const;
	void Action(const std::vector<MFSString> & argv) const;

	TreeCommand();
};

void tree(int depth, std::vector<bool> & flag);