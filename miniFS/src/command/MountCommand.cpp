#include "../../include/command/MountCommand.h"

bool MountCommand::Accept(const MFSString & string) const
{
	if (string == L"mount")
		return true;
	else return false;
}

void MountCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (argv.size() != 1)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		MFSDataSpace * space;
		space = MFSDataSpace::GetActiveDataSpace();
		if (space != NULL)
		{
			space->Close();
			delete space;
		}
		space = new MFSDataSpace(argv[0]);
		MFSDataSpace::SetActiveDataSpace(space);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");;
	}
}

void MountCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"卸载当前活动的数据空间（如果存在），然后挂载将给定的数据空间。\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"mount <name>\n\n");
	point->Log(L"name：要挂载的数据空间的后备存储文件名。\n\n");
	return;
}

MountCommand::MountCommand()
{
}