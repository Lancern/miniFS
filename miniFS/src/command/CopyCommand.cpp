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
			WIN32_FIND_DATAA FindFileData;
			USES_CONVERSION;
			char * path = W2A(argv[0].GetRawString());
			FindFirstFileA(path, &FindFileData);
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("wenjianjia");
			}
			else
			{
				std::ifstream in(path);
				if (!in)
				{
					point->Log(L"文件打开失败");
					return;
				}
				in.seekg(0, std::ios::end);
				std::streampos ps = in.tellg();

				MFSFile * file = space->CreateFile(argv[1], false);
				file->SetFileSize(ps);
				MFSStream *outStream = file->OpenStream();


				std::cout << ps << std::endl;
			}
		}
		else if(!MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
		{

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
	if (space->IsDirectory(space1))
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