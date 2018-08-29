#pragma once
#include "../../miniFS.Core/include/MFSString.h"

class MFSCommand
{

public:
	MFSCommand();

	bool Accept(const MFSString & string) const;
	void Action() const;
};