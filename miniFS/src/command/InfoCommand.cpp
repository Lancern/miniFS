#include "../../include/command/InfoCommand.h"

bool InfoCommand::Accept(const MFSString & string) const
{
	if (string == L"info")
		return true;
	else return false;
}

void InfoCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() != 0)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (!space)
	{
		point->Log(L"��û�й����κοռ�\n");
	}
	else {
		point->Log(L"��ǰ���ݿռ�����ܿռ� ");
		std::wcout << space->GetTotalSpaceInBytes() << std::endl;
		point->Log(L"��ǰ���ݿռ���ÿռ� ");
		std::wcout << space->GetFreeSpaceInBytes() << std::endl;
		point->Log(L"��ǰ���ݿռ����ÿռ� ");
		std::wcout << space->GetTotalSpaceInBytes() - space->GetFreeSpaceInBytes() << std::endl;
	}
}

InfoCommand::InfoCommand()
{
}