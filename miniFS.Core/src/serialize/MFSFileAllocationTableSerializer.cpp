#include "../../include/serialize/MFSFileAllocationTableSerializer.h"
#include "../../include/stream/MFSStreamReader.h"
#include "../../include/stream/MFSStreamWriter.h"

void MFSFileAllocationTableSerializer::Serialize(MFSStream * stream, MFSFileAllocationTable * object)
{
	MFSStreamWriter writer(stream);
	for (uint32_t i = 0; i < object->GetCount(); i++)
		writer.Write(object->_next[i]);
}

MFSFileAllocationTable * MFSFileAllocationTableSerializer::Deserialize(MFSStream * stream)
{
    uint32_t atsize = static_cast<uint32_t>(
        (stream->GetLength() - stream->GetPosition()) / sizeof(uint32_t));
	MFSFileAllocationTable* ret = new MFSFileAllocationTable(atsize);
	MFSStreamReader reader(stream);
	for (uint32_t i = 0; i < ret->GetCount(); i++)
		ret->_next[i] = reader.ReadPODObject<uint32_t>();
	return ret;
}
