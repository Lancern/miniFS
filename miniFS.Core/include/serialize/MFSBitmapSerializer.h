#pragma once

#include "MFSSerializer.h"
#include "../MFSBitmap.h"


class MFSBitmapSerializer
    : public MFSSerializer<MFSBitmap>
{
public:
    void Serialize(MFSStream * stream, MFSBitmap * object) override;
    MFSBitmap * Deserialize(MFSStream * stream) override;
};
