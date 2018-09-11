#include "../../include/command/MkdirCommand.h"

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
	if (argv.size() != 1)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try 
	{
		space->CreateDirectory(argv[0],true);
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