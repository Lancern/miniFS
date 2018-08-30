#pragma once
#include "../../miniFS.Core/include/MFSString.h"
#include <vector>

class MFSCommand
{
public:
	virtual bool Accept(const MFSString & string) const = 0;
	virtual void Action(const std::vector<MFSString> & argv) const = 0;

protected:
	MFSCommand() = default;
};