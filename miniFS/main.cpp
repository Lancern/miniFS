#include <iostream>
#include <vector>
#include <algorithm>
#include "../miniFS.Core/include/MFSString.h"
#include "include/MFSCommand.h"
#include "include/MFSTest.h"
#include "include/io/MFSConsole.h"

int main()
{
	MFSTest command;
	MFSConsole *point = MFSGetDefaultConsole();

	while (1)
	{
		MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
		std::vector<WCHAR> split = { L'|' };
		std::vector<MFSString> subString;
		point->Log(L"miniFS@ ");
		if (space)
			point->Log(space->GetWorkingDirectory());
		point->Log(L">");
		MFSString strInput = point->ReadLine();
		subString = strInput.SplitName(split);
		for (const MFSString & part : subString) {

			MFSTestunit *tmp = command.Chead->link;
			std::vector<WCHAR> splitNew = { L' ', L'\t' };
			std::vector<MFSString> paragrameter;
			paragrameter = part.SplitName(splitNew);
			while (tmp)
			{
				if (tmp->base->Accept(paragrameter[0]))
				{
					paragrameter.erase(paragrameter.begin());
					//std::wcout << paragrameter.back().GetRawString();
					tmp->base->Action(paragrameter);
					break;
				}
				tmp = tmp->link;
			}
			if (tmp == NULL) point->Log(L"÷∏¡Ó ‰»Î”–ŒÛ\n");
		}
	}
	return 0;
}
