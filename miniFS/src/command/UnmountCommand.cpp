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
		printf("��ж�ص�ǰ��ռ�\n");
	}
	else
	{
		//std::wcout << L"������" << std::endl;
		printf("��ǰû�л�ռ�\n");
	}
}

UnmountCommand::UnmountCommand()
{
}