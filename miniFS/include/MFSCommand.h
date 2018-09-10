#pragma once
#include "../../miniFS.Core/include/MFSString.h"
#include <vector>
#include <iostream>
#include "../../miniFS.Core/dist/include/MFSDataSpace.h"
#include "../../miniFS.Core/dist/include/MFSFile.h"
#include "../../miniFS.Core/dist/include/MFSPath.h"
#include "io/MFSConsole.h"

#include "../../miniFS.Core/dist/include/exceptions/MFSDataSpaceNotLoadedException.h"
#include "../../miniFS.Core/dist/include/exceptions/MFSDirectoryNotFoundException.h"
#include "../../miniFS.Core/dist/include/exceptions/MFSFileAlreadyExistException.h"
#include "../../miniFS.Core/dist/include/exceptions/MFSFileNotFoundException.h"
#include "../../miniFS.Core/dist/include/exceptions/MFSInvalidPathException.h"
#include "../../miniFS.Core/dist/include/exceptions/MFSOutOfSpaceException.h"
#include "../../miniFS.Core/dist/include/exceptions/MFSWindowsException.h"

class MFSCommand
{
public:
	virtual bool Accept(const MFSString & string) const = 0;
	virtual void Action(const std::vector<MFSString> & argv) const = 0;

protected:
	MFSCommand() = default;
};