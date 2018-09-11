#include "../../include/command/StatCommand.h"

bool StatCommand::Accept(const MFSString & string) const
{
	if (string == L"stat" || string == L"attr")
		return true;
	else return false;
}

void StatCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 1)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		MFSFile *file = space->OpenFile(argv[0], false);
		if (file->IsHidden()) return;
		point->LogLine(L"创建时间: " + file->GetCreationTime().GetDateTimeString());
		point->LogLine(L"上次访问时间: " + file->GetLastAccessTime().GetDateTimeString());
		point->LogLine(L"上次修改时间: " + file->GetLastModificationTime().GetDateTimeString());
		point->Log(L"大小: ");
		std::wcout << file->GetFileSize() << std::endl;
	}
	catch (MFSException & ex)
	{
		point->LogLine(ex.GetExceptMessage());
	}
}

void StatCommand::Help() const
{
	return;
}

StatCommand::StatCommand()
{
}