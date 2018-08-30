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
		MFSTestunit *tmp = command.Chead->link;
		MFSString strInput = point->ReadLine();
		/*point->SetForegroundColor(MFSConsoleColors::Red);
		point->SetBackgroundColor(MFSConsoleColors::Green);*/
		point->Log(strInput);
		for (int i = 0; i < 14; i++)
		{
			if (tmp->base->Accept(strInput))
			{
				//tmp->base->Action();
				break;
			}
			tmp = tmp->link;
		}
	}
	return 0;
}
