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
		printf("��û�й����κοռ�\n");
	}
	else {
		printf("��ǰ���ݿռ�����ܿռ� ");
		std::wcout << space->GetTotalSpaceInBytes() << std::endl;
		printf("��ǰ���ݿռ���ÿռ� ");
		std::wcout << space->GetFreeSpaceInBytes() << std::endl;
		printf("��ǰ���ݿռ����ÿռ� ");
		std::wcout << space->GetTotalSpaceInBytes() - space->GetFreeSpaceInBytes() << std::endl;
	}
}

InfoCommand::InfoCommand()
{
}