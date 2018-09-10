#include "../../include/command/CopyCommand.h"

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
		point->Log(L"��ǰδ���ؿռ�");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		if (space->IsDirectory(argv[0]))
		{
			Copy(argv[0], argv[1]);
		}
		else
		{
			space->Copy(argv[0],argv[1]);
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
		space->CreateDirectory(space2,true);
		std::vector<MFSString> lis = space->GetFiles(space1);
		Copy(space1 + L"",space2 + L"");
	}
	else
	{
		space->Copy(space1, space2);
	}
}

CopyCommand::CopyCommand()
{
}