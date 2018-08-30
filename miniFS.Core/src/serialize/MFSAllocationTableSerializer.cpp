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
    size_t atsize = static_cast<size_t>(
        (stream->GetLength() - stream->GetPosition()) / sizeof(uint32_t));
	MFSAllocationTable* ret = new MFSAllocationTable(atsize);
	MFSStreamReader reader(stream);
	for (size_t i = 0; i < ret->Size(); i++)
		ret->_next[i] = reader.ReadPODObject<uint32_t>();
	return ret;
}