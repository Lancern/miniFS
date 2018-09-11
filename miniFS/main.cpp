#include <iostream>
#include <vector>
#include <algorithm>
#include "../miniFS.Core/include/MFSString.h"
#include "include/MFSCommand.h"
#include "include/MFSTest.h"
#include "include/io/MFSConsole.h"

void f()
{
	CloseCommand * close = new CloseCommand();
	std::vector<MFSString> a;
	close->Action(a);
	return;
}

int main()
{
	MFSTest command;
	MFSConsole *point = MFSConsole::GetDefaultConsole();

	point->SetHandlerOnExit(&f);

	while (1)
	{
		MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
		std::vector<WCHAR> split = { L' ', L'\t' };
		std::vector<MFSString> subString;
		point->Log(L"miniFS@ ");
		if (space)
			point->Log(space->GetWorkingDirectory());
		point->Log(L">");
		MFSString strInput = point->ReadLine();
		subString = strInput.SplitName(split);
		for (auto paragrameter : subString)
		{
			if (paragrameter.StartsWith(L"\"") && paragrameter.EndsWith(L"\""))
				paragrameter = paragrameter.Substring(1, paragrameter.GetLength() - 2);
		}
		MFSTestunit *tmp = command.Chead->link;
		while (tmp)
		{
			if (tmp->base->Accept(subString[0]))
			{
				subString.erase(subString.begin());
				//std::wcout << paragrameter.back().GetRawString();
				tmp->base->Action(subString);
				break;
			}
			tmp = tmp->link;
		}
		if (tmp == NULL) point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
	}
	return 0;
}
