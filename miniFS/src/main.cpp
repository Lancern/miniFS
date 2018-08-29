#include <iostream>
#include <vector>
#include <algorithm>
#include "../miniFS.Core/include/MFSString.h"
#include "include/MFSCommand.h"

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

typedef struct test_unit
{
	test_unit *link;
	MFS
}test_unit;

int main()
{
	test_unit *head;
	while (1)
	{
		MFSString str_input;
		std::wcin >> str_input;

	}
	return 0;
}
