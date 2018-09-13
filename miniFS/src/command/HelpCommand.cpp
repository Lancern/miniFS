#include "../../include/command/HelpCommand.h"

bool HelpCommand::Accept(const MFSString & string) const
{
	if (string == L"help")
		return true;
	else return false;
}

void HelpCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	if (argv.size() > 1)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	if (argv.size() == 0)
		AllHelp();
	else
	{
		MFSTest command;
		MFSTestunit *tmp = command.Chead->link;
		while (tmp)
		{
			if (tmp->base->Accept(argv[0]))
			{
				//std::wcout << paragrameter.back().GetRawString();
				tmp->base->Help();
				break;
			}
			tmp = tmp->link;
		}
		if (tmp == NULL) point->Log(L"参数输入有误\n");
	}
}

void HelpCommand::AllHelp() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"有关某个命令的详细信息，请键入 help 命令名\n");
	point->Log(L"attr/stat		显示指定的文件的详细信息\n");
	point->Log(L"cd			更换工作目录\n");
	point->Log(L"close/quit		卸载当前活动的数据空间(如果存在)，并退出mini-FS文件系统\n");
	point->Log(L"cp/copy			复制文件或目录\n");
	point->Log(L"create			创建一个新的数据空间\n");
	point->Log(L"dl/del/rm		删除指定的文件或目录\n");
	point->Log(L"dr/dir/ls		显示给定目录下的所有项\n");
	point->Log(L"fmt/format		格式化当前活动的数据空间\n");
	point->Log(L"help			打印mini-FS系统帮助信息\n");
	point->Log(L"hide			切换文件或目录的隐藏标志位\n");
	point->Log(L"info			显示数据空间信息\n");
	point->Log(L"link			创建硬链接\n");
	point->Log(L"map			展示指定文件的所有数据块块号\n");
	point->Log(L"mkdir			创建目录\n");
	point->Log(L"mount			卸载当前活动的数据空间，然后挂载给定的数据空间\n");
	point->Log(L"mv/move			移动文件或目录\n");
	point->Log(L"opt			值行分区碎片整理\n");
	point->Log(L"unmount			保存所有的更改并卸载当前活动的数据空间\n");
	point->Log(L"cls			清空当前屏幕中的显示\n");
	return;
}

void HelpCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"提供 mini-FS 系统的帮助信息\n\n");
	point->Log(L"指令格式\n");
	point->Log(L"help <command>\n\n");
	point->Log(L"command - 可选，显示该命令的帮助信息，不填则显示所有指令的大致信息。\n");
}

HelpCommand::HelpCommand()
{
}