#include "..\..\include\serialize\MFSDirectoryBlockSerializer.h"

MFSDirectoryBlockSerializer::MFSDirectoryBlockSerializer(uint32_t blockSize)
	: _blockSize(blockSize)
{
}

void MFSDirectoryBlockSerializer::Serialize(MFSStream * stream, MFSDirectoryBlock * object)
{
	MFSStreamWriter writer(stream);
    MFSFSDirectoryBlockMasterInfo master = { static_cast<uint32_t>(object->_dir.size()) };
    writer.Write(master);

	for (auto & it : object->_dir)
	{
		writer.Write(it.first);
		writer.Write(it.second);
	}

    // Alignment to the boundary of device block.
    if (stream->CanSeek())
        stream->Seek(MFSStreamSeekOrigin::Relative, _blockSize - object->_usedSize);
    else
    {
        for (uint32_t tmp = object->_usedSize; tmp < _blockSize; ++tmp)
        {
            uint8_t buffer = 0;
            stream->Write(&buffer, sizeof(buffer));
        }
    }
}

MFSDirectoryBlock * MFSDirectoryBlockSerializer::Deserialize(MFSStream * stream)
{
	MFSDirectoryBlock * ret = new MFSDirectoryBlock(_blockSize);
	MFSStreamReader reader(stream);
	MFSFSDirectoryBlockMasterInfo master = reader.ReadPODObject<MFSFSDirectoryBlockMasterInfo>();

	while (master.itemsCount--)
	{
		MFSString key = reader.ReadString();
		MFSFSDirectoryItem value = reader.ReadPODObject<MFSFSDirectoryItem>();
		ret->_dir[key] = value;
        ret->_usedSize += (key.GetLength() + 1) * sizeof(WCHAR);
        ret->_usedSize += sizeof(MFSFSDirectoryItem);
	}

    // Alignment to the boundary of device block.
    if (stream->CanSeek())
        stream->Seek(MFSStreamSeekOrigin::Relative, _blockSize - ret->_usedSize);
    else
    {
        for (uint32_t tmp = ret->_usedSize; tmp < _blockSize; ++tmp)
        {
            uint8_t buffer;
            stream->Read(&buffer, sizeof(buffer), sizeof(buffer));
        }
    }

	return ret;
}
