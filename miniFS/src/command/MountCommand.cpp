#include "../../include/command/MountCommand.h"

bool MountCommand::Accept(const MFSString & string) const
{
	if (string == L"mount")
		return true;
	else return false;
}

void MountCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (argv.size() != 1)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		MFSDataSpace * space;
		space = MFSDataSpace::GetActiveDataSpace();
		if (space != NULL)
		{
			space->Close();
			delete space;
		}
		space = new MFSDataSpace(argv[0]);
		MFSDataSpace::SetActiveDataSpace(space);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");;
	}
}

void MountCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"ж�ص�ǰ������ݿռ䣨������ڣ���Ȼ����ؽ����������ݿռ䡣\n\n");
	point->Log(L"ָ���ʽ��\n");
	point->Log(L"mount <name>\n\n");
	point->Log(L"name��Ҫ���ص����ݿռ�ĺ󱸴洢�ļ�����\n\n");
	return;
}

MountCommand::MountCommand()
{
}