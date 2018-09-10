#include "../../include/command/DirCommand.h"

bool DirCommand::Accept(const MFSString & string) const
{
	if (string == L"dr" || string == L"dir" || string == L"ls")
		return true;
	else return false;
}

void DirCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() > 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	MFSDataSpace * space = MFSDataSpace::GetActiveDataSpace();
	try
	{
		std::vector<MFSString> fileList = space->GetDirectories(space->GetWorkingDirectory());
		for (MFSString file : fileList)
		{
			std::wcout << file.GetRawString() << std::endl;
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