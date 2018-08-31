#pragma once

#include "MFSSerializer.h"
#include "../fs/MFSBlockAllocationBitmap.h"

class MFSBlockAllocationBitmapSerializer
	: public MFSSerializer<MFSBlockAllocationBitmap>
{
public:
	void Serialize(MFSStream * stream, MFSBlockAllocationBitmap * object) override;
	MFSBlockAllocationBitmap * Deserialize(MFSStream * stream) override;
};
