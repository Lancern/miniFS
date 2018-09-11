#include "../../include/command/MoveCommand.h"
#include <Windows.h>
#include <atlconv.h>
#include "../../include/command/CopyCommand.h"
#include "../../include/command/DelCommand.h"

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
		point->Log(L"��ǰδ���ؿռ�\n");
		return;
	}
	if (argv.size() != 2)
	{
		point->Log(L"ָ����������\n");
		return;
	}
	try
	{
		CopyCommand cp;
		DelCommand del;
		if (MFSPath::IsOSPath(argv[0]) && !MFSPath::IsOSPath(argv[1]))
		{
			cp.Cpin(argv[0], argv[1]);
			USES_CONVERSION;
			MFSString order = L"del " + argv[0];
			system(W2A(order.GetRawString()));
		}
		else if (!MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
		{
			cp.Cpout(argv[0], argv[1]);
			del.Del(argv[0]);
		}
		else if (MFSPath::IsOSPath(argv[0]) && MFSPath::IsOSPath(argv[1]))
		{
			USES_CONVERSION;
			MFSString order = L"move " + argv[0] + L" " + argv[1];
			system(W2A(order.GetRawString()));
		}
		else
			space->Move(argv[0], argv[1]);
	}
	catch (MFSException & ex)
	{
		point->Log(ex.GetExceptMessage() + L"\n");;
	}
}

void MoveCommand::Help() const
{
	MFSConsole *point = MFSConsole::GetDefaultConsole();
	point->Log(L"�ƶ��ļ���Ŀ¼��\n\n");
	point->Log(L"ָ���ʽ��\n");
	point->Log(L"mv <src> <dest>\n");
	point->Log(L"move <src> <dest>\n\n");
	point->Log(L"src��mini-FS �ļ�/Ŀ¼���� Windows �ļ�/Ŀ¼����Դ�ļ���Ŀ¼��\n");
	point->Log(L"dest��mini-FS �ļ�/Ŀ¼���� Windows �ļ�/Ŀ¼����Ŀ���ļ���\n\n");
	return;
}

MoveCommand::MoveCommand()
{
}