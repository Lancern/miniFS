#include "../../include/command/LinkCommand.h"

bool LinkCommand::Accept(const MFSString & string) const
{
	if (string == L"link")
		return true;
	else return false;
}

void LinkCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		space->CreateLink(argv[0], argv[1]);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");
	}
}

void LinkCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"����Ӳ����\n\n");
	point->Log(L"ָ���ʽ\n");
	point->Log(L"link <src> <target>\n\n");
	point->Log(L"src��Ҫ���ӵ����ļ���Ŀ¼����\n");
	point->Log(L"target�������ļ���Ŀ¼����\n\n");
}

LinkCommand::LinkCommand()
{
}