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
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 1)
	{
		point->Log(L"指令输入有误\n");
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
	return;
}

MkdirCommand::MkdirCommand()
{
}