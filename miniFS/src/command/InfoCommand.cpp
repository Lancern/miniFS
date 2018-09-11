#include "../../include/command/InfoCommand.h"

bool InfoCommand::Accept(const MFSString & string) const
{
	if (string == L"info")
		return true;
	else return false;
}

void InfoCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 0)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	if (!space)
	{
		point->Log(L"并没有挂载任何空间\n");
	}
	else {
		point->Log(L"当前数据空间可用总空间 ");
		std::wcout << space->GetTotalSpaceInBytes() << std::endl;
		point->Log(L"当前数据空间可用空间 ");
		std::wcout << space->GetFreeSpaceInBytes() << std::endl;
		point->Log(L"当前数据空间已用空间 ");
		std::wcout << space->GetTotalSpaceInBytes() - space->GetFreeSpaceInBytes() << std::endl;
	}
}

void InfoCommand::Help() const
{
	return;
}

InfoCommand::InfoCommand()
{
}