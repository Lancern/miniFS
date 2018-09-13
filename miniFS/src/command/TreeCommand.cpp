#include "../../include/command/TreeCommand.h"

bool TreeCommand::Accept(const MFSString & string) const
{
	if (string == L"tree")
		return true;
	else return false;
}

void TreeCommand::Action(const std::vector<MFSString> & argv) const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	if (space == NULL)
	{
		point->Log(L"当前未挂载空间\n");
		return;
	}
	if (argv.size() != 0)
	{
		point->Log(L"指令输入有误\n");
		return;
	}
	try
	{
		std::vector<bool>flag;
		this->tree(0, flag);
	}
	catch (MFSException & ex)
	{
		point->LogLine(ex.GetExceptMessage());
	}
}

void TreeCommand::tree(int depth, std::vector<bool> & flag) const
{
	MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	MFSString path = space->GetWorkingDirectory();
	std::vector<MFSString>subdir = space->GetDirectories(path);
	while (flag.size() != depth)
		flag.pop_back();
	if (depth == 0)
	{
		for (int i = 0; i < depth; i++)
			point->Log(L"    ");
		point->LogLine(path);
		flag.push_back(1);
	}
	if (subdir.size() == 0) flag.empty();
	for (auto subpath : subdir)
	{
		space->SetWorkingDirectory(subpath);
		if (subpath == subdir.back())
		{
			for (int i = 0; i < depth; i++)
			{
				if (flag[i] == 1)
					point->Log(L"│   ");
				else
					point->Log(L"    ");
			}
			point->Log(L"└── ");
			if (depth == 0)
				flag[0] = 0;
			if (static_cast<int>(flag.size()) > depth)
				flag[depth] = 0;
			else
				flag.push_back(0);
		}
		else
		{
			for (int i = 0; i < depth; i++)
			{
				if (flag[i] == 1)
					point->Log(L"│   ");
				else
					point->Log(L"    ");
			}
			point->Log(L"├── ");
			flag.push_back(1);
		}
		point->LogLine(subpath);
		tree(depth + 1, flag);
		space->SetWorkingDirectory(path);
	}
}

void TreeCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"以图形显示路径的文件夹结构\n\n");
	point->Log(L"指令格式。\n");
	point->Log(L"tree\n\n");
	return;
}

TreeCommand::TreeCommand()
{
}