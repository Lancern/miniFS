#include "../../include/command/CopyCommand.h"

bool CopyCommand::Accept(const MFSString & string) const
{
	if (string == L"copy" || string == L"cp")
		return true;
	else return false;
}

void CopyCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		Copy(argv[0], argv[1]);
	}
	catch (MFSException &ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
	
}

void CopyCommand::Copy(const MFSString & space1, const MFSString & space2) const
{
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space->IsDirectory(space1))
	{
		space->CreateDirectory(space2, true);
		std::vector<MFSString> fileList = space->GetFiles(space1);
		for (MFSString file : fileList)
		{
			Copy(space1 + L"/" + file, space2 + L"/" + file);
		}
		std::vector<MFSString> directoryList = space->GetDirectories(space1);
		for (MFSString file : directoryList)
		{
			Copy(space1 + L"/" + file, space2 + L"/" + file);
		}
	}
	else
	{
		space->Copy(space1, space2);
	}
}

void CopyCommand::Help() const
{
	return;
}

CopyCommand::CopyCommand()
{
}