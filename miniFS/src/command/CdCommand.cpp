#include "../../include/command/CdCommand.h"

bool CdCommand::Accept(const MFSString & string) const
{
	if (string == L"cd")
		return true;
	else return false;
}

void CdCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	//if (space == NULL)
	//{
	//	point->Log(L"当前未挂载空间\n");
	//	return;
	//}
	if (argv.size() > 1)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		if (argv.size() == 0)
			space->SetWorkingDirectory(L"/");
		else
			space->SetWorkingDirectory(argv[0]);
	}
	catch (MFSException &ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");
	}
}

void CdCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"更换工作目录\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"cd <directory>\n\n");
	point->Log(L"directory - 要更换到的工作目录路径。若不指定此参数，则命令将会将工作目录设置为根目录\n");
	point->Log(L"	../表示父目录\n");
	point->Log(L"	./表示当前目录\n\n");
}

CdCommand::CdCommand()
{
}