#include "../../include/command/SetHiddenCommand.h"

bool SetHiddenCommand::Accept(const MFSString & string) const
{
	if (string == L"hide")
		return true;
	else return false;
}

void SetHiddenCommand::Action(const std::vector<MFSString> & argv) const
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
		bool status = space->GetEntryInfo(argv[0]).IsHidden;
		space->SetHidden(argv[0], status^1);
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