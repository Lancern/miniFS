#include "../../include/command/InfoCommand.h"

bool InfoCommand::Accept(const MFSString & string) const
{
	if (string == L"info")
		return true;
	else return false;
}

void InfoCommand::Action(const std::vector<MFSString> & argv) const
{
	if (argv.size() != 0)
	{
		printf("command is wrong\n");
		return;
	}
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (!space)
	{
		printf("并没有挂载任何空间\n");
	}
	else {
		printf("当前数据空间可用总空间 ");
		std::wcout << space->GetTotalSpaceInBytes() << std::endl;
		printf("当前数据空间可用空间 ");
		std::wcout << space->GetFreeSpaceInBytes() << std::endl;
		printf("当前数据空间已用空间 ");
		std::wcout << space->GetTotalSpaceInBytes() - space->GetFreeSpaceInBytes() << std::endl;
	}
}

InfoCommand::InfoCommand()
{
}