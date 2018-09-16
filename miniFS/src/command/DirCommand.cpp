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
	if (!space->IsFormatted())
	{
		point->Log(L"��ǰ�ռ�δ��ʼ��\n");
		return;
	}
	if (argv.size() > 3)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		if (argv.size() > 0)
		{
			
		}
		if (argv.size() == 0)
		{
			int i = 0;
			MFSString pwd = space->GetWorkingDirectory();
			if (!pwd.EndsWith(L"/")) pwd = pwd + L"/";
			std::vector<MFSString> diretoryList = space->GetDirectories(pwd);
			std::sort(diretoryList.begin(), diretoryList.end());
			point->SetForegroundColor(MFSConsoleColors::Cyan);
			for (MFSString diretory : diretoryList)
			{
				if (space->GetEntryInfo(pwd+diretory).IsHidden) continue;
				point->Log(diretory+L"\t");
				i++;
				if (i == 4)
				{
					i = 0;
					point->Log(L"\n");
				}
			}
			std::vector<MFSString> fileList = space->GetFiles(pwd);
			std::sort(fileList.begin(), fileList.end());
			point->SetForegroundColor(MFSConsoleColors::White);
			for (MFSString file : fileList)
			{
				if (space->GetEntryInfo(pwd + file).IsHidden) continue;
				point->Log(file + L"\t");
				i++;
				if (i == 4)
				{
					i = 0;
					point->Log(L"\n");
				}
			}
			if(i)
				point->Log(L"\n");
		}
		else if (argv.size() == 1)
		{
			if (argv[0] == L"-a")
			{
				int i = 0;
				std::vector<MFSString> diretoryList = space->GetDirectories(space->GetWorkingDirectory());
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					i++;
					point->Log(diretory + L"\t");
					if (i == 4)
					{
						i = 0;
						point->Log(L"\n");
					}
				}
				std::vector<MFSString> fileList = space->GetFiles(space->GetWorkingDirectory());
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					i++;
					point->Log(file + L"\t");
					if (i == 4)
					{
						i = 0;
						point->Log(L"\n");
					}
				}
				if (i)
					point->Log(L"\n");
			}
			else if (argv[0] == L"-l")
			{
				MFSString pwd = space->GetWorkingDirectory();
				if (!pwd.EndsWith(L"/")) pwd = pwd + L"/";
				std::vector<MFSString> diretoryList = space->GetDirectories(pwd);
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					MFSEntryInfo info = space->GetEntryInfo(pwd + diretory);
					if (info.IsHidden) continue;
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(pwd);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					MFSEntryInfo info = space->GetEntryInfo(pwd + file);
					if (info.IsHidden) continue;
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					uint64_t size = space->OpenFile(pwd + file, false)->GetFileSize();
					if (size < 1024)
					{
						std::wcout << size;
						point->Log(L"B  ");
					}
					else if (size < 1024 * 1024)
					{
						printf("%.2lf", double(1.0*size / 1024));
						point->Log(L"KB  ");
					}
					else
					{
						printf("%.2lf", double(1.0*size / 1024 / 1024));
						point->Log(L"MB  ");
					}
					point->LogLine(file);
				}
			}
			else if (argv[0] == L"-al" || argv[0] == L"-la")
			{
				MFSString pwd = space->GetWorkingDirectory();
				if (!pwd.EndsWith(L"/")) pwd = pwd + L"/";
				std::vector<MFSString> diretoryList = space->GetDirectories(pwd);
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					MFSEntryInfo info = space->GetEntryInfo(pwd + diretory);
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(pwd);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					MFSEntryInfo info = space->GetEntryInfo(pwd + file);
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					uint64_t size = space->OpenFile(pwd + file, false)->GetFileSize();
					if (size < 1024)
					{
						std::wcout << size;
						point->Log(L"B  ");
					}
					else if (size < 1024 * 1024)
					{
						printf("%.2lf", double(1.0*size / 1024));
						point->Log(L"KB  ");
					}
					else
					{
						printf("%.2lf", double(1.0*size / 1024 / 1024));
						point->Log(L"MB  ");
					}
					point->LogLine(file);
				}
			}
			else
			{
				int i = 0;
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[0]);
				MFSString str = argv[0];
				if (!argv[0].EndsWith(L"/")) str = argv[0] + L"/";
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					if (space->GetEntryInfo(str + diretory).IsHidden) continue;
					i++;
					point->Log(diretory + L"\t");
					if (i == 4)
					{
						i = 0;
						point->Log(L"\n");
					}
				}
				std::vector<MFSString> fileList = space->GetFiles(argv[0]);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					if (space->GetEntryInfo(str + file).IsHidden) continue;
					i++;
					point->Log(file + L"\t");
					if (i == 4)
					{
						i = 0;
						point->Log(L"\n");
					}
				}
				if (i)
					point->Log(L"\n");
			}
		}
		else if (argv.size() == 2)
		{
			if (argv[0] == L"-al" || argv[0] == L"-la")
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[1]);
				MFSString str = argv[1];
				if (!argv[1].EndsWith(L"/")) str = argv[1] + L"/";
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					MFSEntryInfo info = space->GetEntryInfo(str + diretory);
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(argv[1]);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					MFSEntryInfo info = space->GetEntryInfo(str + file);
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					uint64_t size = space->OpenFile(str + file, false)->GetFileSize();
					if (size < 1024)
					{
						std::wcout << size;
						point->Log(L"B  ");
					}
					else if (size < 1024 * 1024)
					{
						printf("%.2lf", double(1.0*size / 1024));
						point->Log(L"KB  ");
					}
					else
					{
						printf("%.2lf", double(1.0*size / 1024 / 1024));
						point->Log(L"MB  ");
					}
					point->LogLine(file);
				}
			}
			else if ((argv[0] == L"-a" && argv[1] == L"-l") || (argv[0] == L"-l" && argv[1] == L"-a"))
			{
				MFSString pwd = space->GetWorkingDirectory();
				if (!pwd.EndsWith(L"/")) pwd = pwd + L"/";
				std::vector<MFSString> diretoryList = space->GetDirectories(pwd);
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					MFSEntryInfo info = space->GetEntryInfo(pwd + diretory);
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(pwd);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					MFSEntryInfo info = space->GetEntryInfo(pwd + file);
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					uint64_t size = space->OpenFile(pwd + file, false)->GetFileSize();
					if (size < 1024)
					{
						std::wcout << size;
						point->Log(L"B  ");
					}
					else if (size < 1024 * 1024)
					{
						printf("%.2lf", double(1.0*size / 1024));
						point->Log(L"KB  ");
					}
					else
					{
						printf("%.2lf", double(1.0*size / 1024 / 1024));
						point->Log(L"MB  ");
					}
					point->LogLine(file);
				}
			}
			else if (argv[0] == L"-a")
			{
				int i = 0;
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[1]);
				MFSString str = argv[1];
				if (!argv[1].EndsWith(L"/")) str = argv[1] + L"/";
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					i++;
					point->Log(diretory + L"\t");
					if (i == 4)
					{
						i = 0;
						point->Log(L"\n");
					}
				}
				std::vector<MFSString> fileList = space->GetFiles(argv[1]);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					i++;
					point->Log(file + L"\t");
					if (i == 4)
					{
						i = 0;
						point->Log(L"\n");
					}
				}
				if (i)
					point->Log(L"\n");
			}
			else if (argv[0] == L"-l")
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[1]);
				MFSString str = argv[1];
				if (!argv[1].EndsWith(L"/")) str = argv[1] + L"/";
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					MFSEntryInfo info = space->GetEntryInfo(str + diretory);
					if (info.IsHidden) continue;
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(argv[1]);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					MFSEntryInfo info = space->GetEntryInfo(str + file);
					if (info.IsHidden) continue;
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  "); 
					uint64_t size = space->OpenFile(str + file, false)->GetFileSize();
					if (size < 1024)
					{
						std::wcout << size;
						point->Log(L"B  ");
					}
					else if (size < 1024 * 1024)
					{
						printf("%.2lf", double(1.0*size / 1024));
						point->Log(L"KB  ");
					}
					else
					{
						printf("%.2lf", double(1.0*size / 1024 / 1024));
						point->Log(L"MB  ");
					}
					point->LogLine(file);
				}
			}
		}
		else if (argv.size() == 3)
		{
			if ((argv[0] == L"-a" && argv[1] == L"-l") || (argv[0] == L"-l" && argv[1] == L"-a"))
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[2]);
				MFSString str = argv[2];
				if (!argv[2].EndsWith(L"/")) str = argv[2] + L"/";
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					MFSEntryInfo info = space->GetEntryInfo(str + diretory);
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(argv[2]);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					MFSEntryInfo info = space->GetEntryInfo(str + file);
					point->Log(info.CreationTime.GetDateTimeString());
					point->Log(L"  ");
					uint64_t size = space->OpenFile(str + file, false)->GetFileSize();
					if (size < 1024)
					{
						std::wcout << size;
						point->Log(L"B  ");
					}
					else if (size < 1024 * 1024)
					{
						printf("%.2lf", double(1.0*size / 1024));
						point->Log(L"KB  ");
					}
					else
					{
						printf("%.2lf", double(1.0*size / 1024 / 1024));
						point->Log(L"MB  ");
					}
					point->LogLine(file);
				}
			}
		}
	}
	catch (MFSException & ex)
	{
		point->LogLine(ex.GetExceptMessage());
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