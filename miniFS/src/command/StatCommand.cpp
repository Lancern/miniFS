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
		MFSFile *file = space->OpenFile(argv[0], false);
		if (file->IsHidden()) return;
		point->LogLine(L"����ʱ��: " + file->GetCreationTime().GetDateTimeString());
		point->LogLine(L"�ϴη���ʱ��: " + file->GetLastAccessTime().GetDateTimeString());
		point->LogLine(L"�ϴ��޸�ʱ��: " + file->GetLastModificationTime().GetDateTimeString());
		point->Log(L"��С: ");
		std::wcout << file->GetFileSize() << std::endl;
	}
	catch (MFSException & ex)
	{
		point->LogLine(ex.GetExceptMessage());
	}
}

void StatCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"��ʾָ�����ļ�����ϸ��Ϣ��\n\n");
	point->Log(L"ָ���ʽ��\n");
	point->Log(L"attr <file>\n");
	point->Log(L"stat <file>\n\n");
	point->Log(L"file��Ҫ��ʾ��ϸ��Ϣ���ļ�����\n\n");
	return;
}

StatCommand::StatCommand()
{
}