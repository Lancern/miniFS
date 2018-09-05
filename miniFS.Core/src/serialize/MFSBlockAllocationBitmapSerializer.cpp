#include "../../include/serialize/MFSBlockAllocationBitmapSerializer.h"
#include "../../include/stream/MFSStreamReader.h"
#include "../../include/stream/MFSStreamWriter.h"

void MFSBlockAllocationBitmapSerializer::Serialize(MFSStream * stream, MFSBlockAllocationBitmap * object)
{
	MFSStreamWriter writer(stream);
	for (uint32_t i = 0; i < object->_bitmap->Size(); i++)
		writer.Write(object->_bitmap->_bitmap[i]);
}

MFSBlockAllocationBitmap * MFSBlockAllocationBitmapSerializer::Deserialize(MFSStream * stream)
{
	MFSBlockAllocationBitmap* ret = new MFSBlockAllocationBitmap(
        static_cast<uint32_t>((stream->GetLength() - stream->GetPosition()) * CHAR_BIT));
	MFSStreamReader reader(stream);
	for (uint32_t i = 0; i < ret->_bitmap->Size(); i++)
		ret->_bitmap->_bitmap[i] = reader.ReadPODObject<uint64_t>();
	return ret;
}
