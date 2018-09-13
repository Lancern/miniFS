#include "../../include/command/DelCommand.h"

bool DelCommand::Accept(const MFSString & string) const
{
	if (string == L"del" || string == L"dl" ||string == L"rm")
		return true;
	else return false;
}

void DelCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (space == NULL)
	{
		point->Log(L"��ǰδ���ؿռ�");
		return;
	}
	if (argv.size() < 1 || argv.size() > 2)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		if (argv.size() == 2 && argv[1] == L"-f") {
			Del(argv[0]);
		}
		else if (argv.size() == 2 && argv[0] == L"-f")
		{
			Del(argv[1]);
		}
		else if(argv.size() == 1)
		{
			if (space->GetEntryInfo(argv[0]).IsDirectory)
			{
				if (space->GetDirectories(argv[0]).size() || space->GetFiles(argv[0]).size())
				{
					point->Log(L"�ļ��в�Ϊ�գ��޷�ɾ��\n");
					return;
				}
				else
					Del(argv[0]);
			}
			Del(argv[0]);
		}
		else
		{
			point->Log(L"ָ����������\n");
		}
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");;
	}
	
}

void DelCommand::Del(const MFSString & path) const
{
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space->GetEntryInfo(path).IsDirectory)
	{
		if (space->GetDirectories(path).size() == 0 && space->GetFiles(path).size() == 0)
		{
			space->Delete(path);
			return;
		}
		else
		{
			std::vector<MFSString> fileList = space->GetFiles(path);
			for (MFSString file : fileList)
			{
				Del(path + L"/" + file);
			}
			std::vector<MFSString> directoryList = space->GetDirectories(path);
			for (MFSString file : directoryList)
			{
				Del(path + L"/" + file);
			}
		}
		space->Delete(path);
	}
	else
	{
		space->Delete(path);
	}
}

void DelCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"ɾ��ָ�����ļ���Ŀ¼\n\n");
	point->Log(L"ָ���ʽ\n");
	point->Log(L"dl [-f] <file>\n");
	point->Log(L"del [-f] <file>\n");
	point->Log(L"rm [-f] <file>\n\n");
	point->Log(L"file��Ҫɾ�����ļ���\n");
	point->Log(L"-f����ѡ����ָ����ѡ��ʱ��ǿ��ɾ���ǿ�Ŀ¼����������Ŀ¼�����ļ���\n\n");
	return;
}

DelCommand::DelCommand()
{
}