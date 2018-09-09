#include "../../include/command/UnmountCommand.h"

bool UnmountCommand::Accept(const MFSString & string) const
{
	if (string == L"unmount")
		return true;
	else return false;
}

void UnmountCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSDataSpace * space;
	space = MFSDataSpace::GetActiveDataSpace();
	if (space != NULL)
	{
		space->Close();
		delete space;
		MFSDataSpace::SetActiveDataSpace(NULL);
		//std::wcout << L"ccc" << std::endl;
		printf("已卸载当前活动空间\n");
	}
	else
	{
		//std::wcout << L"哈哈哈" << std::endl;
		printf("当前没有活动空间\n");
	}
}

UnmountCommand::UnmountCommand()
{
}