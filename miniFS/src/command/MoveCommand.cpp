#include "../../include/command/MoveCommand.h"

bool MoveCommand::Accept(const MFSString & string) const
{
	if (string == L"move" || string == L"mv")
		return true;
	else return false;
}

void MoveCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		space->Move(argv[0], argv[1] + L"/" + argv[0]);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");;
	}
}

void MoveCommand::Help() const
{
	return;
}

MoveCommand::MoveCommand()
{
}