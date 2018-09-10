#include "../../include/command/DelCommand.h"

bool DelCommand::Accept(const MFSString & string) const
{
	if (string == L"del" || string == L"dl" ||string == L"rm")
		return true;
	else return false;
}

void DelCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (space == NULL)
	{
		point->Log(L"��ǰδ���ؿռ�");
		return;
	}
	if (argv.size() < 1 || argv.size() > 2)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		if (argv.size() == 1) {
			space->Delete(argv[0]);
		}
		else
		{

		}
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");;
	}
	
}

DelCommand::DelCommand()
{
}