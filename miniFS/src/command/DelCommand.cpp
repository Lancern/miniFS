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
		point->Log(L"当前未挂载空间");
		return;
	}
	if (argv.size() < 1 || argv.size() > 2)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		if (argv.size() == 2 && argv[2] == L"-f") {
			space->Delete(argv[0]);
		}
		else if(argv.size() == 1)
		{
			Del(argv[0]);
		}
		else
		{
			point->Log(L"指令输入有误\n");
		}
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");;
	}
	
}

void DelCommand::Del(const MFSString & path) const
{
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space->GetEntryInfo(path).IsDirectory)
	{
		if (space->GetDirectories(path).size() == 0 && space->GetFiles(path).size() == 0)
		{
			space->Delete(path);
			return;
		}
		else
		{
			std::vector<MFSString> fileList = space->GetFiles(path);
			for (MFSString file : fileList)
			{
				Del(path + L"/" + file);
			}
			std::vector<MFSString> directoryList = space->GetDirectories(path);
			for (MFSString file : directoryList)
			{
				Del(path + L"/" + file);
			}
		}
		space->Delete(path);
	}
	else
	{
		space->Delete(path);
	}
}

void DelCommand::Help() const
{
	return;
}

DelCommand::DelCommand()
{
}