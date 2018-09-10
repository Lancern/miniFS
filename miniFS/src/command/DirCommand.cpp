#include "../../include/command/DirCommand.h"

bool DirCommand::Accept(const MFSString & string) const
{
	if (string == L"dr" || string == L"dir" || string == L"ls")
		return true;
	else return false;
}

void DirCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() > 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		std::vector<MFSString> diretoryList = space->GetDirectories(space->GetWorkingDirectory());
		std::sort(diretoryList.begin(),diretoryList.end());
		point->SetForegroundColor(MFSConsoleColors::Cyan);
		for (MFSString diretory : diretoryList)
		{
			point->Log(diretory + L"\n");
		}
		std::vector<MFSString> fileList = space->GetFiles(space->GetWorkingDirectory());
		std::sort(fileList.begin(), fileList.end());
		point->SetForegroundColor(MFSConsoleColors::White);
		for (MFSString file : fileList)
		{
			point->Log(file + L"\n");
		}
	}
	catch (MFSInvalidPathException)
	{
		point->Log(L"给定的路径不合法");
	}
	catch (MFSOutOfSpaceException)
	{
		//point->Log("数据空间空间不足，无法完成要求的操作");
	}

}

DirCommand::DirCommand()
{
}