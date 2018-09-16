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
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (!space->IsFormatted())
	{
		point->Log(L"��ǰ�ռ�δ��ʼ��\n");
		return;
	}
	if (argv.size() < 1 || argv.size() > 2)
	{
		point->Log(L"ָ����������\n");
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
	point->Log(L"����Ŀ¼��\n\n");
	point->Log(L"ָ���ʽ��\n");
	point->Log(L"mkdir <dir>\n\n");
	point->Log(L"dir��Ҫ������Ŀ¼·������·�������в����ڵ�Ŀ¼Ҳ�ᱻһ��������\n\n");

	return;
}

MkdirCommand::MkdirCommand()
{
}