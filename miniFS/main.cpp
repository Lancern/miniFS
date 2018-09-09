#include <iostream>
#include <vector>
#include <algorithm>
#include "../miniFS.Core/include/MFSString.h"
#include "include/MFSCommand.h"
#include "../miniFS.Core/include/stream/MFSStream.h"
#include "../miniFS.Core/include/stream/MFSStreamWriter.h"
#include "../miniFS.Core/include/stream/MFSStreamReader.h"
#include "../miniFS.Core/include/stream/MFSNativeFileStream.h"
#include "../miniFS.Core/include/stream/MFSStreamTextReader.h"
#include "../miniFS.Core/include/stream/MFSStreamTextWriter.h"
#include "include/MFSTest.h"
#include "include/io/MFSConsole.h"
#include "../miniFS.Core/dist/include/MFSDataSpace.h"
int main()
{
	MFSTest command;
	MFSConsole *point = MFSGetDefaultConsole();
	//MFSStreamWriter writer();

	while (1)
	{
		MFSDataSpace *space = MFSDataSpace::GetActiveDataSpace();
		std::vector<WCHAR> split = {L'|'};
		std::vector<MFSString> subString;
		std::wcout << L"miniFS:";
		if (space)
			std::wcout << space->GetWorkingDirectory();
		std::wcout << L">";
		MFSString strInput;
		std::wcin >> strInput;
		//test.Read();
		/*point->SetForegroundColor(MFSConsoleColors::Red);
		point->SetBackgroundColor(MFSConsoleColors::Green);*/
		subString = strInput.SplitName(split);
		for (const MFSString & part : subString) {

			MFSTestunit *tmp = command.Chead->link;
			std::vector<WCHAR> splitNew = { L' ', L'\t'};
			std::vector<MFSString> paragrameter;
			paragrameter = part.SplitName(splitNew);
			while(tmp)
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
