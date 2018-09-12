#pragma once
#include "../MFSCommand.h"

class MoveCommand :virtual public MFSCommand
{
public:
	bool Accept(const MFSString & string) const;
	void Action(const std::vector<MFSString> & argv) const;
	void Help() const;
	void DeleteWindow(const MFSString & string) const;
	void MoveWindow(const MFSString & string1, const MFSString & string2) const;

	MoveCommand();
};