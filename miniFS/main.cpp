#include <iostream>
#include <vector>
#include <algorithm>
#include "../miniFS.Core/include/MFSString.h"
#include "include/MFSCommand.h"
#include "../miniFS.Core/include/stream/MFSStream.h"
#include "../miniFS.Core/include/stream/MFSStreamWriter.h"
#include "../miniFS.Core/include/stream/MFSStreamReader.h"
#include "include/MFSTest.h"
#include "include/io/MFSConsole.h"

int main()
{
	MFSTest command;
	MFSConsole *point = MFSGetDefaultConsole();
	MFSStreamWriter writer();
	//while (1)
	//{
	//	std::vector<WCHAR> split = {L'|'};
	//	std::vector<MFSString> subString;
	//	point->Log(L"miniFS>");
	//	MFSString strInput = point->ReadLine();
	//	/*point->SetForegroundColor(MFSConsoleColors::Red);
	//	point->SetBackgroundColor(MFSConsoleColors::Green);*/
	//	subString = strInput.SplitName(split);
	//	for (const MFSString & part : subString) {

	//		MFSTestunit *tmp = command.Chead->link;
	//		std::vector<WCHAR> splitNew = { L' ', L'\t'};
	//		std::vector<MFSString> paragrameter;
	//		paragrameter = part.SplitName(splitNew);
	//		for (int i = 0; i < 14; i++)
	//		{
	//			if (tmp->base->Accept(paragrameter[0]))
	//			{
	//				paragrameter.erase(paragrameter.begin());
	//				tmp->base->Action(paragrameter);
	//				break;
	//			}
	//			tmp = tmp->link;
	//		}
	//	}
	//}
	return 0;
}
