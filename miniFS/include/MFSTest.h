#pragma once
#include "MFSCommand.h"

class MFSTestunit {

public:
	MFSTestunit *link;
	MFSCommand *base;

	MFSTestunit();
};

class MFSTest {

public:
	MFSTestunit *head;

	MFSTest();
};
