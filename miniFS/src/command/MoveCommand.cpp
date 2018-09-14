#include "../../include/command/MoveCommand.h"
#include <Windows.h>
#include <atlconv.h>
#include "../../include/command/CopyCommand.h"
#include "../../include/command/DelCommand.h"

bool MoveCommand::Accept(const MFSString & string) const
{
	if (string == L"move" || string == L"mv")
		return true;
	else return false;
}

void MoveCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (!space->IsFormatted())
	{
		point->Log(L"当前空间未初始化\n");
		return;
	}
	if (argv.size() != 2 && argv.size() != 3)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		CopyCommand cp;
		DelCommand del;
		bool flag = false;
		if (argv.size() == 3)
		{
			if (argv[2] == L"-m")
				flag = true;
			if (MFSPath::IsOSPath(argv[0]) && !MFSPath::IsOSPath(argv[1]))
			{
				try
				{
					cp.Cpin(argv[0], argv[1], flag);
				}
				catch (MFSException & ex)
				{
					space->Delete(argv[1]);
					cp.Cpin(argv[0], argv[1], flag);
					throw;
				}
				DeleteWindow(argv[0]);
			}
			else
				point->LogLine(L"指令输入有误");
		}
		else
		{
			if (MFSPath::IsOSPath(argv[0]) && !MFSPath::IsOSPath(argv[1]))
			{
				cp.Cpin(argv[0], argv[1], 0);
				DeleteWindow(argv[0]);
			}
			else if (!MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
			{
				cp.Cpout(argv[0], argv[1]);
				del.Del(argv[0]);
			}
			else if (MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
			{
				MoveWindow(argv[0], argv[1]);
			}
			else
				space->Move(argv[0], argv[1]);
		}
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
}

void MoveCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"移动文件或目录。\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"mv <src> <dest>\n");
	point->Log(L"move <src> <dest>\n\n");
	point->Log(L"src：mini-FS 文件/目录名或 Windows 文件/目录名。源文件或目录。\n");
	point->Log(L"dest：mini-FS 文件/目录名或 Windows 文件/目录名。目标文件。\n\n");
	return;
}

void MoveCommand::DeleteWindow(const MFSString & string) const
{
	HANDLE hFind;
	WIN32_FIND_DATAW findData;
	MFSString str = string;
	if (!string.EndsWith(L"\\")) str = string + L"\\";
	hFind = FindFirstFileW((str + L"*.*").GetRawString(), &findData);
	if (hFind == (void *)0xffffffffffffffff)
	{
		hFind = FindFirstFileW(str.Substring(0, str.GetLength() - 1).GetRawString(), &findData);
		DeleteFile(str.Substring(0, str.GetLength() - 1).GetRawString());
	}
	else
	{
		do
		{
			MFSString temp = str + (MFSString)findData.cFileName;
			if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
				continue;
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				DeleteWindow(temp);
			}
			else
			{
				DeleteFile(temp.GetRawString());
			}
		} while (FindNextFileW(hFind, &findData));
	}
	FindClose(hFind);
	RemoveDirectory(str.GetRawString());
}

void MoveCommand::MoveWindow(const MFSString & string1, const MFSString & string2) const
{

	HANDLE hFind;
	WIN32_FIND_DATAW findData;
	MFSString str1 = string1;
	MFSString str2 = string2;
	if (!string1.EndsWith(L"\\")) str1 = string1 + L"\\";
	if (!string2.EndsWith(L"\\")) str2 = string2 + L"\\";
	hFind = FindFirstFileW((str1 + L"*.*").GetRawString(), &findData);
	if (hFind == (void *)0xffffffffffffffff)
	{
		MoveFile(str1.Substring(0, str1.GetLength() - 1).GetRawString(), str2.Substring(0, str2.GetLength() - 1).GetRawString());
	}
	else
	{
		CreateDirectoryW(str2.GetRawString(), NULL);
		do
		{
			MFSString temp1 = str1 + (MFSString)findData.cFileName;
			MFSString temp2 = str2 + (MFSString)findData.cFileName;
			if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
				continue;
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				MoveWindow(temp1, temp2);
			}
			else
			{
				MoveFile(temp1.GetRawString(), temp2.GetRawString());
			}
		} while (FindNextFileW(hFind, &findData));
	}
	FindClose(hFind);
	RemoveDirectory(str1.GetRawString());
}

MoveCommand::MoveCommand()
{
}