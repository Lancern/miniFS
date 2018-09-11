#include "../../include/command/MoveCommand.h"

bool MoveCommand::Accept(const MFSString & string) const
{
	if (string == L"move" || string == L"mv")
		return true;
	else return false;
}

void MoveCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		space->Move(argv[0], argv[1]);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");;
	}
}

void MoveCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"移动文件或目录。\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"mv <src> <dest>\n");
	point->Log(L"move <src> <dest>\n\n");
	point->Log(L"src：mini-FS 文件/目录名或 Windows 文件/目录名。源文件或目录。\n");
	point->Log(L"dest：mini-FS 文件/目录名或 Windows 文件/目录名。目标文件。\n\n");
	return;
}

MoveCommand::MoveCommand()
{
}