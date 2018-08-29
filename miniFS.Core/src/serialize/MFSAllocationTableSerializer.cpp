#include "../../include/serialize/MFSAllocationTableSerializer.h"
#include "../../include/stream/MFSStreamReader.h"
#include "../../include/stream/MFSStreamWriter.h"

void MFSAllocationTableSerializer::Serialize(MFSStream * stream, MFSAllocationTable * object)
{
	MFSStreamWriter writer(stream);
	for (size_t i = 0; i < object->Size(); i++)
		writer.Write(object->_next[i]);
}

MFSAllocationTable * MFSAllocationTableSerializer::Deserialize(MFSStream * stream)
{
	MFSAllocationTable* ret = new MFSAllocationTable((stream->GetLength() - stream->GetPosition()) / sizeof(int32_t));
	MFSStreamReader reader(stream);
	for (size_t i = 0; i < ret->Size(); i++)
		ret->_next[i] = reader.ReadPODObject<int32_t>();
	return ret;
}
