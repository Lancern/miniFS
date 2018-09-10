#include "../../include/command/DirCommand.h"

bool DirCommand::Accept(const MFSString & string) const
{
	if (string == L"dr" || string == L"dir" || string == L"ls")
		return true;
	else return false;
}

void DirCommand::Action(const std::vector<MFSString> & argv) const
{
	if (argv.size() != 0)
	{
		printf("command is wrong\n");
		return;
	}
	MFSDataSpace * space = MFSDataSpace::GetActiveDataSpace();
	std::vector<MFSString> fileList = space->GetDirectories(space->GetWorkingDirectory());
	for (MFSString file : fileList)
	{
		std::wcout << file.GetRawString() << std::endl;
	}
}

DirCommand::DirCommand()
{
}