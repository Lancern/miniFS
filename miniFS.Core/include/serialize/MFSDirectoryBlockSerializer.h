#pragma once

#include "../serialize/MFSSerializer.h"
#include "../fs/MFSDirectoryBlock.h"
#include "../stream/MFSStreamReader.h"
#include "../stream/MFSStreamWriter.h"

class MFSDirectoryBlockSerializer
	: public MFSSerializer<MFSDirectoryBlock>
{
public:
	MFSDirectoryBlockSerializer(uint32_t blockSize);

	void Serialize(MFSStream * stream, MFSDirectoryBlock * object) override;
	MFSDirectoryBlock * Deserialize(MFSStream * stream) override;

private:
	uint32_t _blockSize;
};
