#include "../../include/command/SetHiddenCommand.h"

bool SetHiddenCommand::Accept(const MFSString & string) const
{
	if (string == L"set")
		return true;
	else return false;
}

void SetHiddenCommand::Action(const std::vector<MFSString> & argv) const
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
	if (argv[1] != L"0" && argv[1] != L"1" && argv[1] != L"true" && argv[1] != L"false")
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		MFSFile *file = space->OpenFile(argv[0], false);
		if (argv[1] == L"1" || argv[1] == L"true")
		{
			file->SetHidden(true);
		}
		else if (argv[1] == L"0" || argv[1] == L"false")
		{
			file->SetHidden(false);
		}
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
}

void SetHiddenCommand::Help() const
{
	return;
}

SetHiddenCommand::SetHiddenCommand()
{
}