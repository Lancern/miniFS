#include "../../include/command/MkdirCommand.h"

bool MkdirCommand::Accept(const MFSString & string) const
{
	if (string == L"mkdir")
		return true;
	else return false;
}

void MkdirCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 1)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try 
	{
		space->CreateDirectory(argv[0],true);
	}
	catch(MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");
	}
}

void MkdirCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"创建目录。\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"mkdir <dir>\n\n");
	point->Log(L"dir：要创建的目录路径。该路径上所有不存在的目录也会被一并创建。\n\n");

	return;
}

MkdirCommand::MkdirCommand()
{
}