#pragma once

#include "MFSSerializer.h"
#include "../fs/MFSFileAllocationTable.h"


class MFSFileAllocationTableSerializer
	: public MFSSerializer<MFSFileAllocationTable>
{
public:
	void Serialize(MFSStream * stream, MFSFileAllocationTable * object) override;
	MFSFileAllocationTable * Deserialize(MFSStream * stream) override;
};