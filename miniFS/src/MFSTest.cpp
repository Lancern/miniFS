#include "../include/MFSTest.h"

MFSTestunit::MFSTestunit()
{
	link = NULL;
	base = NULL;
}

MFSTest::MFSTest() :Chead()
{
	CloseCommand *c1 = new CloseCommand;
	CopyCommand *c2 = new CopyCommand;
	CreateCommand *c3 = new CreateCommand;
	DelCommand *c4 = new DelCommand;
	DirCommand *c5 = new DirCommand;
	FormatCommand *c6 = new FormatCommand;
	HelpCommand *c7 = new HelpCommand;
	LinkCommand *c8 = new LinkCommand;
	MkdirCommand *c9 = new MkdirCommand;
	MountCommand *c10 = new MountCommand;
	MoveCommand *c11 = new MoveCommand;
	StatCommand *c12 = new StatCommand;
	TpCommand *c13 = new TpCommand;
	UnmountCommand *c14 = new UnmountCommand;
	InfoCommand *c15 = new InfoCommand;
	MFSCommand * list[15] = {c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15};
	Chead = new MFSTestunit;
	for (int i = 0; i < 15; i++)
	{
		MFSTestunit *tmp = new MFSTestunit;
		tmp->link = Chead->link;
		Chead->link = tmp;
		tmp->base = list[i];
	}
}