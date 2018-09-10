#include "../../include/command/DelCommand.h"

bool DelCommand::Accept(const MFSString & string) const
{
	if (string == L"del" || string == L"dl" ||string == L"rm")
		return true;
	else return false;
}

void DelCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() < 1 || argv.size() > 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
		if (space == NULL)
		{
			point->Log(L"未挂载数据空间");
			return;
		}
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