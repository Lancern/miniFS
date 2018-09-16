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
	if (!space->IsFormatted())
	{
		point->Log(L"当前空间未初始化\n");
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
		std::wcout << space->GetTotalSpaceInBytes() <<std::endl;
		point->Log(L"当前数据空间可用空间 ");
		std::wcout << space->GetFreeSpaceInBytes()  << std::endl;
		point->Log(L"当前数据空间已用空间 ");
		std::wcout << (space->GetTotalSpaceInBytes() - space->GetFreeSpaceInBytes())  << std::endl;
	}
}

void InfoCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"显示数据空间的总空间，已用空间和剩余空间的信息\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"info\n\n");
}

InfoCommand::InfoCommand()
{
}