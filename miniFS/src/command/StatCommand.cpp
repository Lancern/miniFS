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
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() != 1)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		MFSEntryInfo info = space->GetEntryInfo(argv[0]);
		if (info.IsHidden) return;
		point->LogLine(L"����ʱ��: " + info.CreationTime.GetDateTimeString());
		point->LogLine(L"�ϴη���ʱ��: " + info.LastAccessTime.GetDateTimeString());
		point->LogLine(L"�ϴ��޸�ʱ��: " + info.LastModificationTime.GetDateTimeString());
		if (!info.IsDirectory)
		{
			point->Log(L"��С: ");
			MFSFile *file = space->OpenFile(argv[0], false);
			std::wcout << file->GetFileSize() << std::endl;
		}
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