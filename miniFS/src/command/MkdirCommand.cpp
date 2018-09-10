#include "../../include/command/MkdirCommand.h"

bool MkdirCommand::Accept(const MFSString & string) const
{
	if (string == L"mkdir")
		return true;
	else return false;
}

void MkdirCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
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
		MFSDataSpace *space;
		space = MFSDataSpace::GetActiveDataSpace();
		space->CreateDirectory(argv[0]);
	}
	catch(MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");
	}
}

MkdirCommand::MkdirCommand()
{
}