#include "..\..\include\serialize\MFSDirectoryBlockSerializer.h"

MFSDirectoryBlockSerializer::MFSDirectoryBlockSerializer(size_t blockSize)
	: _blockSize(blockSize)
{
}

void MFSDirectoryBlockSerializer::Serialize(MFSStream * stream, MFSDirectoryBlock * object)
{
	MFSStreamWriter writer(stream);
	writer.Write(object->_dictUsed);
	writer.Write(object->_heapUsed);
	for (auto& it : object->_dict)
		writer.Write(it);
	for (uint32_t i = 0; i < object->_heapUsed; i++)
		writer.Write(*(object->_heapBase + i));
}

MFSDirectoryBlock * MFSDirectoryBlockSerializer::Deserialize(MFSStream * stream)
{
	MFSDirectoryBlock* ret = new MFSDirectoryBlock(_blockSize - 8);
	MFSStreamReader reader(stream);
	ret->_dictUsed = reader.ReadPODObject<uint32_t>();
	ret->_heapUsed = reader.ReadPODObject<uint32_t>();
	for (uint32_t i = 0; i < ret->_dictUsed; i++)
		ret->_dict.push_back(reader.ReadPODObject<MFSFSDirectoryItem>());
	for (uint32_t i = 0; i < ret->_heapUsed; i++)
		*(ret->_heapBase + i) = reader.ReadPODObject<WCHAR>();
}
