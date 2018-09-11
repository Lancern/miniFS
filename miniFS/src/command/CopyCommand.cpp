#include "../../include/command/CopyCommand.h"
#include <atlconv.h>
#include <windows.h>
#include <fstream>
#include <iostream>

bool CopyCommand::Accept(const MFSString & string) const
{
	if (string == L"copy" || string == L"cp")
		return true;
	else return false;
}

bool CopyCommand::Cpin(const MFSString & argv_0, const MFSString & argv_1) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	WIN32_FIND_DATAW FindFileData;
	FindFirstFileW(argv_0.GetRawString(), &FindFileData);
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		printf("文件夹");
		
	}
	else
	{
		std::ifstream in(argv_0.GetRawString(), std::ios::binary);
		if (!in)
		{
			point->Log(L"文件以二进制形式打开失败");
			return false;
		}

		in.seekg(0, std::ios::end);
		std::streampos ps = in.tellg();
		in.seekg(0, std::ios::beg);

		MFSFile * file = space->CreateFile(argv_1, false);
		file->SetFileSize(ps);
		MFSStream *outStream = file->OpenStream();

		char Buffer[257];
		uint32_t n = ps % 256;
		uint64_t m = ps / 256 + (n ? 1 : 0);
		for (uint64_t i = 0; i <= m; i++)
		{
			if (i != m)
			{
				in.read(Buffer, 256);
				outStream->Write(Buffer, 256);
			}
			else {
				in.read(Buffer, n);
				outStream->Write(Buffer, n);
			}
		}

		outStream->Close();
	}

	return true;
}

bool CopyCommand::Cpout(const MFSString & argv_0, const MFSString & argv_1) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	WIN32_FIND_DATAW FindFileData;
	FindFirstFileW(argv_0.GetRawString(), &FindFileData);
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		space->CreateDirectory(argv_1, true);

	}
	else
	{
		std::ofstream out(argv_1.GetRawString(), std::ios::binary);
		if (!out)
		{
			point->Log(L"文件以二进制形式打开失败");
			return false;
		}

		MFSFile * file = space->OpenFile(argv_0, false);
		uint64_t ps = file->GetFileSize();
		MFSStream *outStream = file->OpenStream();

		char Buffer[257];
		uint32_t n = ps % 256;
		uint64_t m = ps / 256 + (n ? 1 : 0);
		for (uint64_t i = 0; i < m; i++)
		{
			if (i != m - 1)
			{
				outStream->Read(Buffer, 256, 256);
				out.write(Buffer, 256);
			}
			else {
				outStream->Read(Buffer, 256, n);
				out.write(Buffer, n);
			}
		}
		outStream->Close();
	}
	return true;
}

void CopyCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		if (MFSPath::IsOSPath(argv[0]) && !MFSPath::IsOSPath(argv[1]))
		{
			Cpin(argv[0], argv[1]);
		}
		else if (!MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
		{
			Cpout(argv[0], argv[1]);
		}
		else if (MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
		{
			USES_CONVERSION;
			MFSString order = L"copy " + argv[0] + L" " + argv[1];
			system(W2A(order.GetRawString()));
		}
		else
		{
			Copy(argv[0], argv[1]);
		}

	}
	catch (MFSException &ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
	
}

void CopyCommand::Copy(const MFSString & space1, const MFSString & space2) const
{
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space->GetEntryInfo(space1).IsDirectory)
	{
		space->CreateDirectory(space2, true);
		std::vector<MFSString> fileList = space->GetFiles(space1);
		for (MFSString file : fileList)
		{
			Copy(space1 + L"/" + file, space2 + L"/" + file);
		}
		std::vector<MFSString> directoryList = space->GetDirectories(space1);
		for (MFSString file : directoryList)
		{
			Copy(space1 + L"/" + file, space2 + L"/" + file);
		}
	}
	else
	{
		space->Copy(space1, space2);
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