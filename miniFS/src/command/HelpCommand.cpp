#include "../../include/command/HelpCommand.h"

bool HelpCommand::Accept(const MFSString & string) const
{
	if (string == L"help")
		return true;
	else return false;
}

void HelpCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (argv.size() > 1)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	if (argv.size() == 0)
		AllHelp();
	else
	{
		MFSTest command;
		MFSTestunit *tmp = command.Chead->link;
		while (tmp)
		{
			if (tmp->base->Accept(argv[0]))
			{
				//std::wcout << paragrameter.back().GetRawString();
				tmp->base->Help();
				break;
			}
			tmp = tmp->link;
		}
		if (tmp == NULL) point->Log(L"������������\n");
	}
}

void HelpCommand::AllHelp() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"�й�ĳ���������ϸ��Ϣ������� help ������\n");
	point->Log(L"attr/stat		��ʾָ�����ļ�����ϸ��Ϣ\n");
	point->Log(L"cd			��������Ŀ¼\n");
	point->Log(L"close/quit		ж�ص�ǰ������ݿռ�(�������)�����˳�mini-FS�ļ�ϵͳ\n");
	point->Log(L"cp/copy			�����ļ���Ŀ¼\n");
	point->Log(L"create			����һ���µ����ݿռ�\n");
	point->Log(L"dl/del/rm		ɾ��ָ�����ļ���Ŀ¼\n");
	point->Log(L"dr/dir/ls		��ʾ����Ŀ¼�µ�������\n");
	point->Log(L"fmt/format		��ʽ����ǰ������ݿռ�\n");
	point->Log(L"help			��ӡmini-FSϵͳ������Ϣ\n");
	point->Log(L"hide			�л��ļ���Ŀ¼�����ر�־λ\n");
	point->Log(L"info			��ʾ���ݿռ���Ϣ\n");
	point->Log(L"link			����Ӳ����\n");
	point->Log(L"map			չʾָ���ļ����������ݿ���\n");
	point->Log(L"mkdir			����Ŀ¼\n");
	point->Log(L"mount			ж�ص�ǰ������ݿռ䣬Ȼ����ظ��������ݿռ�\n");
	point->Log(L"mv/move			�ƶ��ļ���Ŀ¼\n");
	point->Log(L"opt			ֵ�з�����Ƭ����\n");
	point->Log(L"unmount			�������еĸ��Ĳ�ж�ص�ǰ������ݿռ�\n");
	point->Log(L"cls			��յ�ǰ��Ļ�е���ʾ\n");
	return;
}

void HelpCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"�ṩ mini-FS ϵͳ�İ�����Ϣ\n\n");
	point->Log(L"ָ���ʽ\n");
	point->Log(L"help <command>\n\n");
	point->Log(L"command - ��ѡ����ʾ������İ�����Ϣ����������ʾ����ָ��Ĵ�����Ϣ��\n");
}

HelpCommand::HelpCommand()
{
}