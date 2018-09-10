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
		std::vector<uint32_t>blockId;
		MFSFile *file = space->OpenFile(argv[0], false);
		blockId = file->GetAllBlocksId();
		point->LogLine(L"Block id:");
		for (auto id : blockId)
		{
			std::wcout << ' !' << id;
		}
		point->Log(L"\n");
	}
	catch (MFSException & ex)
	{
		point->LogLine(ex.GetExceptMessage());
	}
}

MapCommand::MapCommand()
{
}