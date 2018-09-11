#include "../../include/command/MapCommand.h"

bool MapCommand::Accept(const MFSString & string) const
{
	if (string == L"map")
		return true;
	else return false;
}

void MapCommand::Action(const std::vector<MFSString> & argv) const
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
		std::vector<uint32_t>blockId;
		MFSFile *file = space->OpenFile(argv[0], false);
		blockId = file->GetAllBlocksId();
		point->LogLine(L"Block id:");
		if (blockId.size() == 0)
			point->Log(L"None");
		for (auto id : blockId)
		{
			std::wcout << id;
		}
		point->Log(L"\n");
	}
	catch (MFSException & ex)
	{
		point->LogLine(ex.GetExceptMessage());
	}
}

void MapCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"չʾָ���ļ����������ݿ��š�\n\n");
	point->Log(L"ָ���ʽ\n");
	point->Log(L"map <file>\n\n");
	point->Log(L"չʾָ���ļ����������ݿ��š�\n");
	point->Log(L"file��Ŀ���ļ�����\n\n");
	return;
}

MapCommand::MapCommand()
{
}