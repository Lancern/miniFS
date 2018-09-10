#include "../../include/command/TouchCommand.h"

bool TouchCommand::Accept(const MFSString & string) const
{
	if (string == L"touch")
		return true;
	else return false;
}

void TouchCommand::Action(const std::vector<MFSString> & argv) const
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
		MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
		MFSFile * file = space->CreateFile(argv[0], false);
	}
	catch(MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
}

TouchCommand::TouchCommand()
{
}