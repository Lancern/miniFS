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
		tree(0, flag);
	}
	catch (MFSException & ex)
	{
		point->LogLine(ex.GetExceptMessage());
	}
}

void tree(int depth, std::vector<bool> & flag)
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

TreeCommand::TreeCommand()
{
}