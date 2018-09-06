#include "../../include/serialize/MFSBlockAllocationBitmapSerializer.h"
#include "../../include/stream/MFSStreamReader.h"
#include "../../include/stream/MFSStreamWriter.h"

void MFSBlockAllocationBitmapSerializer::Serialize(MFSStream * stream, MFSBlockAllocationBitmap * object)
{
	MFSStreamWriter writer(stream);
	for (auto pack : object->_bitmap->_bitmap)
		writer.Write(pack);
}

MFSBlockAllocationBitmap * MFSBlockAllocationBitmapSerializer::Deserialize(MFSStream * stream)
{
	MFSBlockAllocationBitmap* ret = new MFSBlockAllocationBitmap(
        static_cast<uint32_t>((stream->GetLength() - stream->GetPosition()) * CHAR_BIT));
	MFSStreamReader reader(stream);
	for (auto & pack : ret->_bitmap->_bitmap)
		pack = reader.ReadPODObject<uint64_t>();
    ret->LocateNextFreeBlock();
	return ret;
}
