#include "../../include/command/LinkCommand.h"

bool LinkCommand::Accept(const MFSString & string) const
{
	if (string == L"link")
		return true;
	else return false;
}

void LinkCommand::Action(const std::vector<MFSString> & argv) const
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
		space->CreateLink(argv[0], argv[1]);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");
	}
}

void LinkCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"创建硬链接\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"link <src> <target>\n\n");
	point->Log(L"src：要链接到的文件或目录名。\n");
	point->Log(L"target：链接文件或目录名。\n\n");
}

LinkCommand::LinkCommand()
{
}