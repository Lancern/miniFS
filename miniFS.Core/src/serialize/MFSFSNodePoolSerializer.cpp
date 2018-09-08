#include "../../include/serialize/MFSFSNodePoolSerializer.h"


void MFSFSNodePoolSerializer::Serialize(MFSStream * stream, MFSFSNodePool * object)
{
    stream->Write(object->_pool.get(), object->_nodesCount * sizeof(MFSFSEntryMeta));
}

MFSFSNodePool * MFSFSNodePoolSerializer::Deserialize(MFSStream * stream)
{
    uint32_t nodesCount = static_cast<uint32_t>(
        (stream->GetLength() - stream->GetPosition()) / sizeof(MFSFSEntryMeta));
    MFSFSNodePool * pool = new MFSFSNodePool(nodesCount);

    stream->Read(pool->_pool.get(), nodesCount * sizeof(MFSFSEntryMeta), nodesCount * sizeof(MFSFSEntryMeta));
    pool->_alloc = pool->LocateNextFreeNode();

    return pool;
}
