#pragma once

#include "MFSSerializer.h"
#include "../fs/MFSAllocationTable.h"


class MFSAllocationTableSerializer
	: public MFSSerializer<MFSAllocationTable>
{
public:
	void Serialize(MFSStream * stream, MFSAllocationTable * object) override;
	MFSAllocationTable * Deserialize(MFSStream * stream) override;
};