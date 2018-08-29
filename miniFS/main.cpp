#include <iostream>
#include <vector>
#include <algorithm>
#include "../miniFS.Core/include/MFSString.h"
#include "include/MFSCommand.h"
#include "include/MFSTest.h"

int 
std::istream & operator >> (std::wistream & input, MFSString & string)
{
	const auto terminators = { '\t', ' ', '\n' };

	std::vector<WCHAR> buffer;
	while (!input.eof())
	{
		WCHAR ch;
		input.get(ch);
		buffer.push_back(ch);

		if (std::find(terminators.begin(), terminators.end(), ch))
		{
			buffer.pop_back();
			break;
		}
	}

	string = MFSString(buffer.data(), buffer.size());
}

int main()
{
	MFSTest command;
	MFSTestunit *tmp = command.head->link;
	while (1)
	{
		MFSString strInput;
		std::cout << "miniFS>";
		std::wcin >> strInput;
		for (int i = 0; i < 15; i++)
		{
			if (tmp->base->Accept(strInput))
			{
				tmp->base->Action();
				break;
			}
			tmp = tmp->link;
		}
	}
	return 0;
}
