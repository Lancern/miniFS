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
		std::wcout << L"miniFS>";
		if (space)
			std::wcout << space->GetWorkingDirectory().GetRawString();

		//MFSString strInput = point->ReadLine();
		std::vector<WCHAR> str;
		while (1)
		{
			WCHAR temp;
			DWORD read;
			ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &temp, 1, &read, NULL);
			str.push_back(temp);
			if (str.back() == L'\n')
			{
				str.pop_back();
				if (!str.empty() && str.back() == L'\r')
					str.pop_back();
				break;
			}
		}
		MFSString strInput(str.data(), static_cast<uint32_t>(str.size()));
		//test.Read();
		/*point->SetForegroundColor(MFSConsoleColors::Red);
		point->SetBackgroundColor(MFSConsoleColors::Green);*/
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
					tmp->base->Action(paragrameter);
					break;
				}
				tmp = tmp->link;
			}
			if (tmp == NULL) std::cout << "input error!" << std::endl;
		}
	}
	return 0;
}
