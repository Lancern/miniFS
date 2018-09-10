#include "../../include/command/CreateCommand.h"

bool CreateCommand::Accept(const MFSString & string) const
{
	if (string == L"create")
		return true;
	else return false;
}

void CreateCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSGetDefaultConsole();
	if (argv.size() < 1 || argv.size() > 2)
	{
		point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
		return;
	}
	try
	{
		MFSDataSpace * space;
		if(argv.size() == 1)
			space = MFSDataSpace::CreateDataSpace(argv[0], 1024 * 1024 * 1024);
		else 
			space = MFSDataSpace::CreateDataSpace(argv[1], argv[0].ParseInteger<int>());
		space->Close();
	}
	catch (MFSWindowsException & ex)
	{
		std::wcerr << L"FUCK: " << ex.GetErrorCode() << L" " << ex.GetExceptMessage().GetRawString() << std::endl;
	}
	//catch (MFSException & ex)
	//{
	//	std::wcerr << ex.GetExceptMessage().GetRawString() << std::endl;
	//}
}

CreateCommand::CreateCommand()
{
}