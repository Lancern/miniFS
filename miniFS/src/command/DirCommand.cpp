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
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() > 2)
	{
		point->Log(L"ָ����������\n");
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
		point->Log(L"������·�����Ϸ�");
	}
	catch (MFSOutOfSpaceException)
	{
		//point->Log("���ݿռ�ռ䲻�㣬�޷����Ҫ��Ĳ���");
	}

}

DirCommand::DirCommand()
{
}