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

void DirCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"显示给定目录下的所有目录项。\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"dr [-a] [-l] [<path>]\n");
	point->Log(L"dir [-a] [-l] [<path>]\n");
	point->Log(L"ls [-a] [-l] [<path>]\n\n");
	point->Log(L"-a：可选。指定该选项时将显示隐藏文件和目录。\n");
	point->Log(L"-l：可选。指定该选项时将显示更加详细的信息。\n");
	point->Log(L"path：可选。要显示目录项的目录。若不指定此参数，则命令将显示当前工作目录下的所有目录项。\n");
	return;
}

DirCommand::DirCommand()
{
}