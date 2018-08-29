#pragma once
#include "../../miniFS.Core/include/MFSString.h"

class MFSCommand
{
	MFSCommand();

	bool Accept(const MFSString & string) const;
	void Action() const;
};