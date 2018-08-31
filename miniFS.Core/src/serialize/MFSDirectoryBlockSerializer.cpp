#include "..\..\include\serialize\MFSDirectoryBlockSerializer.h"

MFSDirectoryBlockSerializer::MFSDirectoryBlockSerializer(size_t blockSize)
	: _blockSize(blockSize)
{
}

void MFSDirectoryBlockSerializer::Serialize(MFSStream * stream, MFSDirectoryBlock * object)
{
	MFSStreamWriter writer(stream);
	writer.Write(object->_dir.size());
	for (auto& it : object->_dir)
	{
		writer.Write(it.first);
		writer.Write(it.second);
	}
}

MFSDirectoryBlock * MFSDirectoryBlockSerializer::Deserialize(MFSStream * stream)
{
	MFSDirectoryBlock* ret = new MFSDirectoryBlock(_blockSize - 8);
	MFSStreamReader reader(stream);
	uint32_t cnt = reader.ReadPODObject<uint32_t>();
	while (cnt--)
	{
		MFSString key = reader.ReadString();
		MFSFSDirectoryItem value = reader.ReadPODObject<MFSFSDirectoryItem>();
		ret->_dir[key] = value;
	}
	return ret;
}
