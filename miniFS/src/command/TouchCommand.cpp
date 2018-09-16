#include "../../include/command/TouchCommand.h"

bool TouchCommand::Accept(const MFSString & string) const
{
	if (string == L"touch")
		return true;
	else return false;
}

void TouchCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() > 2)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		MFSFile * file = space->CreateFile(argv[0], false);
		if (argv.size() == 2)
		{
			try
			{
				file->SetFileSize(argv[1].ParseInteger<int>());
			}
			catch (MFSException)
			{
				space->Delete(argv[0]);
				throw;
			}
		}
			
		delete file;
	}
	catch(MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");
	}
}

void TouchCommand::Help() const
{
	return;
}

TouchCommand::TouchCommand()
{
}