#include "../include/MFSTest.h"

MFSTestunit::MFSTestunit()
{
	link = NULL;
	base = NULL;
}

MFSTest::MFSTest()
{
	CloseCommand c1;
	MFSCommand * list[15];
	for (int i = 0; i < 15; i++)
	{
		MFSTestunit tmp;
		tmp.link = head.link;
		head.link = &tmp;
		tmp.base = list[i];
	}
}