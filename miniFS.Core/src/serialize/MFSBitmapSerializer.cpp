#include "../../include/serialize/MFSBitmapSerializer.h"
#include "../../include/stream/MFSStreamReader.h"
#include "../../include/stream/MFSStreamWriter.h"

void MFSBitmapSerializer::Serialize(MFSStream * stream, MFSBitmap * object)
{
	MFSStreamWriter writer(stream);
	for (size_t i = 0; i < object->Size(); i++)
		writer.Write(object->_bitmap[i]);
}

MFSBitmap * MFSBitmapSerializer::Deserialize(MFSStream * stream)
{
	MFSBitmap* ret = new MFSBitmap((stream->GetLength() - stream->GetPosition()) * CHAR_BIT);
	MFSStreamReader reader(stream);
	for (size_t i = 0; i < ret->Size(); i++)
		ret->_bitmap[i] = reader.ReadPODObject<uint64_t>();
	return ret;
}
