#include "../../include/command/CreateCommand.h"

bool CreateCommand::Accept(const MFSString & string) const
{
	if (string == L"create")
		return true;
	else return false;
}

void CreateCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (argv.size() < 1 || argv.size() > 2)
	{
		point->Log(L"指令输入有误\n");
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
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage()+L"\n");;
	}
}

void CreateCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"创建一个数据空间\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"create [size] <name>\n\n");
	point->Log(L"name：数据空间的后备存储文件名。\n");
	point->Log(L"size：可选。数据空间的大小。以 MB 计算。默认为 1024。不得小于 128，不得超过 4096。\n\n");
	return;
}

CreateCommand::CreateCommand()
{
}