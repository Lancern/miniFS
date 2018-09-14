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
	delete close;
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
		for (auto & paragrameter : subString)
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
		if (tmp == NULL) point->Log(L"指令输入有误\n");
	}
	return 0;
}


//
//
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <algorithm>
//#include <stdlib.h>
//#include <string>
//#include "../miniFS.Core/include/MFSString.h"
//#include "include/MFSCommand.h"
//#include "include/MFSTest.h"
//#include "include/io/MFSConsole.h"
//
//void f()
//{
//	CloseCommand * close = new CloseCommand();
//	std::vector<MFSString> a;
//	close->Action(a);
//	delete close;
//	return;
//}
//
//int wmain(int argc, wchar_t *argv[])
//{
//	MFSTest command;
//	MFSConsole *point = MFSConsole::GetDefaultConsole();
//	MFSDataSpace *space;
//
//	point->SetHandlerOnExit(&f);
//
//	for (int i = 0; ; i++)
//	{
//		space = MFSDataSpace::GetActiveDataSpace();    
//		std::vector<WCHAR> split = { L'#', L'\t' };
//		std::vector<MFSString> subString;
//
//		point->Log(L"miniFS@ ");
//		if (space)
//			point->Log(space->GetWorkingDirectory());
//		point->Log(L">");
//		//MFSString strInput = argv[i];
//		MFSString strInput;
//		if (i == 0 || i == 1)
//		{
//			if (i == 0)
//				strInput = L"mount#D:\\data.dat";
//			else
//				strInput = L"fmt";
//		}
//		else {
//			if (rand() % 2 == 1)
//			{
//				strInput = L"touch#";
//				strInput = strInput.Concat(MFSString((wchar_t *)&std::to_string(i)));
//				strInput = strInput.Concat(L"#1");
//			}
//			else {
//				strInput = L"mkdir#";
//				strInput = strInput.Concat(MFSString(i));
//			}
//		}
//		subString = strInput.SplitName(split);
//		for (auto s : subString)
//			point->Log(s + L" ");
//		point->Log(L"\n");
//		for (auto & paragrameter : subString)
//		{
//			if (paragrameter.StartsWith(L"\"") && paragrameter.EndsWith(L"\""))
//				paragrameter = paragrameter.Substring(1, paragrameter.GetLength() - 2);
//		}
//		std::cout << "i = " << i << std::endl;
//		MFSTestunit *tmp = command.Chead->link;
//		while (tmp)
//		{
//			if (tmp->base->Accept(subString[0]))
//			{
//				subString.erase(subString.begin());
//				//std::wcout << paragrameter.back().GetRawString();
//				tmp->base->Action(subString);
//				break;
//			}
//			tmp = tmp->link;
//		}
//		if (tmp == NULL) point->Log(L"指令输入有误\n");
//	}
//	delete point;
//	return 0;
//}
