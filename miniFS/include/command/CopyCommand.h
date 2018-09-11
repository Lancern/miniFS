#pragma once
#include "../MFSCommand.h"

class CopyCommand :virtual public MFSCommand
{
public:
	bool Accept(const MFSString & string) const;
	bool Cpin(const MFSString & argv_0, const MFSString & argv_1) const;
	bool Cpout(const MFSString & argv_0, const MFSString & argv_1) const;
	void Action(const std::vector<MFSString> & argv) const;
	void Help() const;

	void Copy(const MFSString & space1, const MFSString & space2) const;
	CopyCommand();
};