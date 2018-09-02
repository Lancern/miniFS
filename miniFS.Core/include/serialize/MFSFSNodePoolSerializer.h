#include "MFSSerializer.h"
#include "../fs/MFSFSNodePool.h"


class MFSFSNodePoolSerializer
    : public MFSSerializer<MFSFSNodePool>
{
public:
    void Serialize(MFSStream * stream, MFSFSNodePool * object) override;
    MFSFSNodePool * Deserialize(MFSStream * stream) override;
};
