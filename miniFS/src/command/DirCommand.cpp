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
	if (argv.size() > 3)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		if (argv.size() == 0)
		{
			MFSString pwd = space->GetWorkingDirectory();
			if (!pwd.EndsWith(L"/")) pwd = pwd + L"/";
			std::vector<MFSString> diretoryList = space->GetDirectories(pwd);
			std::sort(diretoryList.begin(), diretoryList.end());
			point->SetForegroundColor(MFSConsoleColors::Cyan);
			for (MFSString diretory : diretoryList)
			{
				if (space->GetEntryInfo(pwd+diretory).IsHidden) continue;
				point->LogLine(diretory);
			}
			std::vector<MFSString> fileList = space->GetFiles(pwd);
			std::sort(fileList.begin(), fileList.end());
			point->SetForegroundColor(MFSConsoleColors::White);
			for (MFSString file : fileList)
			{
				if (space->GetEntryInfo(pwd + file).IsHidden) continue;
				point->LogLine(file);
			}
		}
		else if (argv.size() == 1)
		{
			if (argv[0] == L"-a")
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(space->GetWorkingDirectory());
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(space->GetWorkingDirectory());
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					point->LogLine(file);
				}
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
					std::wcout << space->OpenFile(pwd + file, false)->GetFileSize() << L"  ";
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
					std::wcout << space->OpenFile(pwd + file, false)->GetFileSize() << L"  ";
					point->LogLine(file);
				}
			}
			else
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[0]);
				MFSString str;
				if (!argv[0].EndsWith(L"/")) str = argv[0] + L"/";
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					if (space->GetEntryInfo(str + diretory).IsHidden) continue;
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(argv[0]);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					if (space->GetEntryInfo(str + file).IsHidden) continue;
					point->LogLine(file);
				}
			}
		}
		else if (argv.size() == 2)
		{
			if (argv[0] == L"-al" || argv[0] == L"-la")
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[1]);
				MFSString str;
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
					std::wcout << space->OpenFile(str + file, false)->GetFileSize() << L"  ";
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
					std::wcout << space->OpenFile(pwd + file, false)->GetFileSize() << L"  ";
					point->LogLine(file);
				}
			}
			else if (argv[0] == L"-a")
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[1]);
				MFSString str;
				if (!argv[1].EndsWith(L"/")) str = argv[1] + L"/";
				std::sort(diretoryList.begin(), diretoryList.end());
				point->SetForegroundColor(MFSConsoleColors::Cyan);
				for (MFSString diretory : diretoryList)
				{
					point->LogLine(diretory);
				}
				std::vector<MFSString> fileList = space->GetFiles(argv[1]);
				std::sort(fileList.begin(), fileList.end());
				point->SetForegroundColor(MFSConsoleColors::White);
				for (MFSString file : fileList)
				{
					point->LogLine(file);
				}
			}
			else if (argv[0] == L"-l")
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[1]);
				MFSString str;
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
					std::wcout << space->OpenFile(str + file, false)->GetFileSize() << L"  ";
					point->LogLine(file);
				}
			}
		}
		else if (argv.size() == 3)
		{
			if ((argv[0] == L"-a" && argv[1] == L"-l") || (argv[0] == L"-l" && argv[1] == L"-a"))
			{
				std::vector<MFSString> diretoryList = space->GetDirectories(argv[2]);
				MFSString str;
				if (!argv[1].EndsWith(L"/")) str = argv[2] + L"/";
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
					std::wcout << space->OpenFile(str + file, false)->GetFileSize() << L"  ";
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