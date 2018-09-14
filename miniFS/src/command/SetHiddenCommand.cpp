#include "../../include/command/SetHiddenCommand.h"

bool SetHiddenCommand::Accept(const MFSString & string) const
{
	if (string == L"hide")
		return true;
	else return false;
}

void SetHiddenCommand::Action(const std::vector<MFSString> & argv) const
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
	if (argv.size() != 1)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		bool status = space->GetEntryInfo(argv[0]).IsHidden;
		space->SetHidden(argv[0], status^1);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
}

void SetHiddenCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"切换文件或目录的隐藏标志位。若目标文件或目录已被设置隐藏标志位，\n则这些隐藏标志位将被移除；否则隐藏标志位将被添加。\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"hide <path>\n\n");
	point->Log(L"path：要切换隐藏标志位的文件或目录。\n\n");
	return;
}

SetHiddenCommand::SetHiddenCommand()
{
}