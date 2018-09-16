#include "../../include/command/MkdirCommand.h"
#include <string>
bool MkdirCommand::Accept(const MFSString & string) const
{
	if (string == L"mkdir")
		return true;
	else return false;
}

void MkdirCommand::Action(const std::vector<MFSString> & argv) const
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
	if (argv.size() < 1 || argv.size() > 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try 
	{
		if(argv.size() == 1)
			space->CreateDirectory(argv[0],true);
		else
		{
			wchar_t a[5];
			for (int i = 1; i < argv[1].ParseInteger<int>() ; i++)
			{
				wsprintfW(a, L"%d", i);
				//MFSFile * file = space->CreateFile(a, false);
				//file->SetFileSize(1);


				space->CreateDirectory(a,true);
				if(i%1000 == 0)
					point->LogLine(a);

				//delete file;
				//if (i % 1000 == 0)
				//{
				//	space->Close();
				//	delete space;
				//	MFSDataSpace::SetActiveDataSpace(NULL);
				//	space = new MFSDataSpace(L"F:\files\test1.dat");
				//	MFSDataSpace::SetActiveDataSpace(space);
				//	point->LogLine(L"mount");
				//}
			}
		}
	}
	catch(MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");
	}
}

void MkdirCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"创建目录。\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"mkdir <dir>\n\n");
	point->Log(L"dir：要创建的目录路径。该路径上所有不存在的目录也会被一并创建。\n\n");

	return;
}

MkdirCommand::MkdirCommand()
{
}