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

void DirCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"��ʾ����Ŀ¼�µ�����Ŀ¼�\n\n");
	point->Log(L"ָ���ʽ\n");
	point->Log(L"dr [-a] [-l] [<path>]\n");
	point->Log(L"dir [-a] [-l] [<path>]\n");
	point->Log(L"ls [-a] [-l] [<path>]\n\n");
	point->Log(L"-a����ѡ��ָ����ѡ��ʱ����ʾ�����ļ���Ŀ¼��\n");
	point->Log(L"-l����ѡ��ָ����ѡ��ʱ����ʾ������ϸ����Ϣ��\n");
	point->Log(L"path����ѡ��Ҫ��ʾĿ¼���Ŀ¼������ָ���˲������������ʾ��ǰ����Ŀ¼�µ�����Ŀ¼�\n");
	return;
}

DirCommand::DirCommand()
{
}