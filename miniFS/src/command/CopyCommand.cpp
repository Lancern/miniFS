#include "../../include/command/CopyCommand.h"
#include <atlconv.h>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <io.h>


bool CopyCommand::Accept(const MFSString & string) const
{
	if (string == L"copy" || string == L"cp")
		return true;
	else return false;
}

bool CopyCommand::Cpin(const MFSString & argv_0, const MFSString & argv_1, const bool flag) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	WIN32_FIND_DATAW FindFileData;
	HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD coord;
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(hand, &cursor);
	HANDLE hFind = FindFirstFileW(argv_0.GetRawString(), &FindFileData);
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		space->CreateDirectory(argv_1, true);
		_wfinddata_t fileinfo;
		intptr_t handle = 0;
		if ((handle = _wfindfirst((argv_0 + L"\\*").GetRawString(), &fileinfo)) != -1)
		{
			do
			{
				if (wcscmp(fileinfo.name, L".") == 0 || wcscmp(fileinfo.name, L"..") == 0)
					continue;
				MFSString file = MFSString(fileinfo.name);
				try
				{
					Cpin(argv_0 + L"\\" + file, argv_1 + L"/" + file, flag);
				}
				catch (const MFSFileAlreadyExistException)
				{
					if (flag)
					{
						space->Delete(argv_1 + L"/" + file);
						Cpin(argv_0 + L"\\" + file, argv_1 + L"/" + file, flag);
					}
					else
					{
						FindClose(hFind);
						_findclose(handle);
						throw MFSFileAlreadyExistException(argv_0 + L"\\" + file);
					}
				}
			} while (_wfindnext(handle, &fileinfo) == 0);
		}
		_findclose(handle);
	}
	else
	{
		std::ifstream in(argv_0.GetRawString(), std::ios::binary);
		if (!in.is_open())
		{
			point->Log(L"文件以二进制形式打开失败\n");
			return false;
		}
		cursor.bVisible = false;
		SetConsoleCursorInfo(hand, &cursor);
		GetConsoleScreenBufferInfo(hand, &csbi);
		coord.X = 0;
		coord.Y = csbi.dwCursorPosition.Y;
		SetConsoleCursorPosition(hand, coord);
		point->Log(L"\n");
		point->LogLine(argv_0.GetRawString());
		in.seekg(0, std::ios::end);
		std::streampos ps = in.tellg();
		in.seekg(0, std::ios::beg);
		MFSFile * file;
		try
		{
			file = space->CreateFile(argv_1, false);
		}
		catch(const MFSFileAlreadyExistException)
		{
			in.close();
			throw;
		}
		try
		{
			file->SetFileSize(ps);
		}
		catch (MFSException )
		{
			space->Delete(file->GetFileName());
			throw;
		}
		
		MFSStream *outStream = file->OpenStream();

		char * Buffer = new char[4194305];
		uint32_t n = ps % 4194304;
		uint64_t m = ps / 4194304 + (n ? 1 : 0);
		if (m == 0)
		{
			point->Log(L"100.00%  |>>>>>>>>>>>>>>>>>>>>|  0B");
		}
		for (uint64_t i = 0; i < m; i++)
		{
			if (i != m - 1)
			{
				in.read(Buffer, 4194304);
				outStream->Write(Buffer, 4194304);
			}
			else {
				in.read(Buffer, n);
				outStream->Write(Buffer, n);
			}
			GetConsoleScreenBufferInfo(hand, &csbi);
			coord.X = 0;
			coord.Y = csbi.dwCursorPosition.Y;
			SetConsoleCursorPosition(hand, coord);
			if (1)
			{
				if (m == 0)
				{
					point->Log(L"100.00%  |>>>>>>>>>>>>>>>>>>>>|  0B");
				}
				else
				{
					printf("%.2lf%%", 100 * (1.0*(i + 1) / m));
					point->Log(L"  |");
					for (int j = 0; j < 20; j++)
					{
						if (j <= 20 * 1.0 *(i + 1) / m)
							point->Log(L">");
						else
							point->Log(L" ");
					}
					point->Log(L"|  ");
					if (ps < 1024)
					{
						std::wcout << ps;
						point->Log(L"B");
					}
					else if (ps < 1024 * 1024)
					{
						printf("%.2lf", double(1.0*ps / 1024));
						point->Log(L"KB");
					}
					else
					{
						printf("%.2lf", double(1.0*ps / 1024 / 1024));
						point->Log(L"MB");
					}
				}
			}
		}
		delete[] Buffer;
		point->Log(L"\n");
		in.close();
		outStream->Close();

	}
	FindClose(hFind);
	cursor.bVisible = true;
	SetConsoleCursorInfo(hand, &cursor);
	return true;
}

bool CopyCommand::Cpout(const MFSString & argv_0, const MFSString & argv_1) const
{
	HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD coord;
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(hand, &cursor);
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space->GetEntryInfo(argv_0).IsDirectory)
	{
		CreateDirectoryW(argv_1.GetRawString(),NULL);
		std::vector<MFSString> fileList = space->GetFiles(argv_0);
		for (MFSString file : fileList)
		{
			Cpout(argv_0 + L"/" + file, argv_1 + L"\\" + file);
		}
		std::vector<MFSString> directoryList = space->GetDirectories(argv_0);
		for (MFSString file : directoryList)
		{
			Cpout(argv_0 + L"/" + file, argv_1 + L"\\" + file);
		}
	}
	else
	{
		GetConsoleScreenBufferInfo(hand, &csbi);
		coord.X = 0;
		coord.Y = csbi.dwCursorPosition.Y;
		SetConsoleCursorPosition(hand, coord);
		point->Log(L"\n");
		point->LogLine(argv_0.GetRawString());
		MFSFile * file = space->OpenFile(argv_0, false);
		uint64_t ps = file->GetFileSize();
		MFSStream *outStream = file->OpenStream();

		std::ofstream out(argv_1.GetRawString(), std::ios::binary);
		if (!out.is_open())
		{
			point->Log(L"文件以二进制形式打开失败\n");
			return false;
		}
		cursor.bVisible = false;
		SetConsoleCursorInfo(hand, &cursor);
		char * Buffer = new char[4194305];
		uint32_t n = ps % 4194304;
		uint64_t m = ps / 4194304 + (n ? 1 : 0);
		if (m == 0)
		{
			point->Log(L"100.00%  |>>>>>>>>>>>>>>>>>>>>|  0B");
		}
		for (uint64_t i = 0; i < m; i++)
		{
			if (i != m - 1)
			{
				outStream->Read(Buffer, 4194304, 4194304);
				out.write(Buffer, 4194304);
			}
			else {
				outStream->Read(Buffer, 4194304, n);
				out.write(Buffer, n);
			}
			if (1)
			{
				cursor.bVisible = false;
				SetConsoleCursorInfo(hand, &cursor);
				GetConsoleScreenBufferInfo(hand, &csbi);
				coord.X = 0;
				coord.Y = csbi.dwCursorPosition.Y;
				SetConsoleCursorPosition(hand, coord);
				if (m == 0 && n == 0)
				{
					point->Log(L"100.00%  |>>>>>>>>>>>>>>>>>>>>|  0B");
				}
				else
				{
					printf("%.2lf%%", 100 * (1.0 * (i + 1) / m));
					point->Log(L"  |");
					for (int j = 0; j < 20; j++)
					{
						if (j <= 20 * 1.0 *(i + 1) / m)
							point->Log(L">");
						else
							point->Log(L" ");
					}
					point->Log(L"|  ");
					if (ps < 1024)
					{
						std::wcout << ps;
						point->Log(L"B");
					}
					else if (ps < 1024 * 1024)
					{
						printf("%.2lf", double(1.0*ps / 1024));
						point->Log(L"KB");
					}
					else
					{
						printf("%.2lf", double(1.0*ps / 1024 / 1024));
						point->Log(L"MB");
					}
				}
			}
		}
		point->Log(L"\n");
		out.close();
		outStream->Close();
	}
	cursor.bVisible = true;
	SetConsoleCursorInfo(hand, &cursor);
	return true;
}

void CopyCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	bool flag = false;
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间");
		return;
	}
	if (argv.size() < 2 || argv.size() > 3)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		if (argv.size() == 3)
		{
			if (argv[2] == L"-m")
			{
				flag = true;
			}
			else {
				point->Log(L"指令输入有误\n");
				return;
			}
		}
		if (MFSPath::IsOSPath(argv[0]) && !MFSPath::IsOSPath(argv[1]))
		{
			if (_waccess(argv[0].GetRawString(), 0) == -1)
			{
				point->Log(L"路径不存在\n");
				return;
			}
			try
			{
				Cpin(argv[0], argv[1], flag);
			}
			catch (const MFSFileAlreadyExistException)
			{
				if (flag)
				{
					space->Delete(argv[1]);
					Cpin(argv[0], argv[1], flag);
				}
				else
				{
					throw;
				}
			}
		}
		else if (!MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
		{
			if (!space->Exist(argv[0]))
			{
				point->Log(L"路径不存在\n");
				return;
			}
			MFSString str = argv[0];
			if (str.EndsWith(L"/"))
				str = str.Substring(0, str.GetLength() - 1);
			Cpout(str, argv[1]);
		}
		else if (MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
		{
			point->Log(L"指令输入有误\n");
			return;
		}
		else
		{
			Copy(argv[0], argv[1], flag);
		}

	}
	catch (MFSException &ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
	
}

void CopyCommand::Copy(const MFSString & space1, const MFSString & space2, const bool flag) const
{
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space->GetEntryInfo(space1).IsDirectory)
	{
		space->CreateDirectory(space2, true);
		std::vector<MFSString> fileList = space->GetFiles(space1);
		for (MFSString file : fileList)
		{
			try
			{
				Copy(space1 + L"/" + file, space2 + L"/" + file, flag);
			}
			catch (MFSException )
			{
				space->Delete(space2 + L"/" + file);
				throw;
			}
		}
		std::vector<MFSString> directoryList = space->GetDirectories(space1);
		for (MFSString file : directoryList)
		{
			Copy(space1 + L"/" + file, space2 + L"/" + file, flag);
		}
	}
	else
	{
		try
		{
			space->Copy(space1, space2);
		}
		catch (const MFSFileAlreadyExistException)
		{
			if (flag)
			{
				space->Delete(space2);
				space->Copy(space1, space2);
			}
			else
				throw;
		}
		catch (MFSException)
		{
			space->Delete(space2);
			throw;
		}
	}
}

void CopyCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"复制文件或目录\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"cp <src> <dest>\n");
	point->Log(L"copy <src> <dest>\n\n");
	point->Log(L"src：mini-FS 文件/目录名或 Windows 文件/目录名。源文件或目录。\n");
	point->Log(L"dest：mini-FS 文件/目录名或 Windows 文件/目录名。目标文件或目录。\n\n");
}

CopyCommand::CopyCommand()
{
}