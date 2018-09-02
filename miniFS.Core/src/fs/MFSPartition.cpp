#include "../../include/fs/MFSPartition.h"
#include "../../include/stream/MFSMemoryStream.h"
#include "../../include/serialize/MFSBlockAllocationBitmapSerializer.h"
#include "../../include/serialize/MFSFileAllocationTableSerializer.h"
#include "../../include/stream/MFSStreamWriter.h"
#include "../../include/MFSDateTime.h"



#define CEIL_DIV(a, b)      ((a) / (b) + (a) % (b) ? 1 : 0)

#define BLOCK_CHAIN_TAIL      0xFFFFFFFF


MFSPartition::MFSPartition(MFSBlockDevice * device)
    : _device(device), _validDevice(true)
{
    LoadDevice(device);
}

MFSPartition::~MFSPartition()
{
    Close();
}

MFSBlockDevice * MFSPartition::GetDevice() const
{
    return _device.get();
}

bool MFSPartition::IsValidDevice() const
{
    return _validDevice;
}

bool MFSPartition::IsRaw() const
{
    return _master.magicSeq != MFS_MAGIC_SEQ;
}

UINT64 MFSPartition::GetTotalSpaceInBytes() const
{
    return static_cast<UINT64>(_master.totalBlocks) * _device->GetBlockSize();
}

UINT64 MFSPartition::GetFreeSpaceInBytes() const
{
    return static_cast<UINT64>(_master.freeBlocks) * _device->GetBlockSize();
}

void MFSPartition::BuildFileSystem()
{
    // Build partition master record.
    _master.magicSeq = MFS_MAGIC_SEQ;
    _master.mfsVer = MFS_VER;
    _master.totalBlocks = static_cast<DWORD>(_device->GetBlocksCount());

    DWORD blockSize = _device->GetBlockSize();
    DWORD babBlocksCount = _master.totalBlocks / CHAR_BIT / blockSize;
    DWORD fatBlocksCount = _master.totalBlocks * sizeof(DWORD) / blockSize;
    DWORD fsnodePoolBlocksCount = _master.totalBlocks * sizeof(MFSFSEntryMeta) / blockSize;
    DWORD fsblocksCount = 1 + babBlocksCount + fatBlocksCount + fsnodePoolBlocksCount;    // mini-FS 占用的块数量

    _master.freeBlocks = _master.totalBlocks - fsblocksCount;

    // Build block allocation bitmap.
    _blockAllocation.reset(new MFSBlockAllocationBitmap(_device->GetBlocksCount()));
    for (DWORD blockId = 0; blockId < fsblocksCount; ++blockId)
        _blockAllocation->Set(blockId);

    // Build file allocation table.
    _blockChain.reset(new MFSFileAllocationTable(_device->GetBlocksCount()));
    DWORD babBlockOffset = 1;
    DWORD fatBlockOffset = babBlockOffset + babBlocksCount;
    DWORD fsnodePoolBlockOffset = fatBlockOffset + fatBlocksCount;

    _blockChain->Set(0, BLOCK_CHAIN_TAIL);

    for (DWORD i = 0; i < babBlocksCount; ++i)
        _blockChain->Set(babBlockOffset + i, babBlockOffset + i + 1);
    _blockChain->Set(babBlockOffset + babBlocksCount - 1, BLOCK_CHAIN_TAIL);

    for (DWORD i = 0; i < fatBlocksCount; ++i)
        _blockChain->Set(fatBlockOffset + i, fatBlockOffset + i + 1);
    _blockChain->Set(fatBlockOffset + fatBlocksCount - 1, BLOCK_CHAIN_TAIL);

    for (DWORD i = 0; i < fsnodePoolBlocksCount; ++i)
        _blockChain->Set(fsnodePoolBlockOffset + i, fsnodePoolBlockOffset + i + 1);
    _blockChain->Set(fsnodePoolBlockOffset + fsnodePoolBlocksCount - 1, BLOCK_CHAIN_TAIL);

    // Initialize fsnode pool.
    _fsnodePool.reset(new MFSFSEntryMeta[static_cast<DWORD>(_device->GetBlocksCount())]);
    // Initialize the first fsnode as the node for the root directory.
    _fsnodePool[0].common.flags = 0 | MFS_FSENTRY_FLAG_PROTECTED;
    _fsnodePool[0].common.firstBlockId = BLOCK_CHAIN_TAIL;

    uint64_t timestamp = MFSGetCurrentTimestamp();
    MFSGetInteger64Struct(&_fsnodePool[0].common.creationTimestamp, timestamp);
    MFSGetInteger64Struct(&_fsnodePool[0].common.lastAccessTimestamp, timestamp);
    MFSGetInteger64Struct(&_fsnodePool[0].common.lastModTimestamp, timestamp);

    _fsnodePool[0].common.refCount = 1;

    // TODO: Implement MFSPartition::BuildFileSystem.
}

MFSFSEntry * MFSPartition::GetRoot() const
{
    if (!_validDevice || IsRaw())
        return nullptr;

    // TODO: Implement MFSPartition::GetRoot().

    return nullptr;
}

void MFSPartition::Flush()
{
    if (!_validDevice)
        return;

    MFSBlockStream stream(_device.get());
    MFSStreamWriter writer(&stream);
    
    // Write master info.
    writer.Write(_master);
    stream.Seek(MFSStreamSeekOrigin::Begin, _device->GetBlockSize());

    // Write block allocation bitmap.
    MFSBlockAllocationBitmapSerializer().Serialize(&stream, _blockAllocation.get());

    // Write file allocation table.
    MFSFileAllocationTableSerializer().Serialize(&stream, _blockChain.get());

    // Write fsnode pool.
    stream.Write(_fsnodePool.get(), static_cast<DWORD>(_device->GetBlocksCount() * sizeof(MFSFSEntryMeta)));

    stream.Flush();
    stream.Close();
}

void MFSPartition::Close()
{
    if (_validDevice && _device)
    {
        Flush();
        _device->Close();
        _device.release();
        _validDevice = false;
    }
}

void MFSPartition::LoadDevice(MFSBlockDevice * device)
{
    typedef bool (MFSPartition::* InitializeFunction)(MFSBlockStream *);

    InitializeFunction initSequence[] = {
        &MFSPartition::LoadMasterInfo,
        &MFSPartition::LoadBlockAllocationManager,
        &MFSPartition::LoadAllocationTable,
        &MFSPartition::LoadFSNodePool
    };

    // Initiate initialization sequence.
    MFSBlockStream blockStream(device);
    for (InitializeFunction pf : initSequence)
    {
        if (!(this->*pf)(&blockStream))
            return;
    }
}

bool MFSPartition::LoadMasterInfo(MFSBlockStream * deviceStream)
{
    LPVOID buffer = VirtualAlloc(NULL, deviceStream->GetDeviceBlockSize(), 
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    DWORD read = deviceStream->Read(buffer, 0, deviceStream->GetDeviceBlockSize());

    bool ret = (read == deviceStream->GetDeviceBlockSize());
    if (ret)
        _master = *reinterpret_cast<MFSFSMasterInfo *>(buffer);

    VirtualFree(buffer, 0, MEM_RELEASE);

    if (!ret)
        _validDevice = false;
    
    return ret && _master.magicSeq == MFS_MAGIC_SEQ;
}

bool MFSPartition::LoadBlockAllocationManager(MFSBlockStream * deviceStream)
{
    DWORD dwBabByteSize = CEIL_DIV(deviceStream->GetDeviceBlockSize(), CHAR_BIT);
    LPVOID buffer = VirtualAlloc(NULL, dwBabByteSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    DWORD read = deviceStream->Read(buffer, dwBabByteSize, dwBabByteSize);
    bool ret = (read == dwBabByteSize);

    if (ret)
    {
        MFSMemoryStream memStream(buffer, dwBabByteSize);
        _blockAllocation.reset(MFSBlockAllocationBitmapSerializer().Deserialize(&memStream));
    }

    VirtualFree(buffer, 0, MEM_RELEASE);

    if (!ret)
        _validDevice = false;

    return ret;
}

bool MFSPartition::LoadAllocationTable(MFSBlockStream * deviceStream)
{
    DWORD dwFatByteSize = static_cast<DWORD>(deviceStream->GetDeviceBlocksCount()) * sizeof(UINT32);
    LPVOID buffer = VirtualAlloc(NULL, dwFatByteSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    DWORD read = deviceStream->Read(buffer, dwFatByteSize, dwFatByteSize);
    bool ret = (read == dwFatByteSize);

    if (ret)
    {
        MFSMemoryStream memStream(buffer, dwFatByteSize);
        _blockChain.reset(MFSFileAllocationTableSerializer().Deserialize(&memStream));
    }

    VirtualFree(buffer, 0, MEM_RELEASE);

    if (!ret)
        _validDevice = false;

    return ret;
}

bool MFSPartition::LoadFSNodePool(MFSBlockStream * deviceStream)
{
    DWORD dwFsnodepoolByteSize = static_cast<DWORD>(deviceStream->GetDeviceBlocksCount()) * sizeof(MFSFSEntryMeta);
    MFSFSEntryMeta * buffer = new MFSFSEntryMeta[static_cast<size_t>(deviceStream->GetDeviceBlocksCount())];

    DWORD read = deviceStream->Read(buffer, dwFsnodepoolByteSize, dwFsnodepoolByteSize);
    bool ret = (read == dwFsnodepoolByteSize);
    
    if (ret)
        _fsnodePool.reset(buffer);
    else
    {
        _validDevice = false;
        delete[] buffer;
    }

    return ret;
}

MFSPartition::ChainedBlockStream * MFSPartition::OpenBlockStream(DWORD firstBlock)
{
    return new ChainedBlockStream(this, firstBlock);
}

MFSPartition::ChainedBlockStream * MFSPartition::OpenBlockStream(DWORD firstBlock, UINT64 length)
{
    return new ChainedBlockStream(this, firstBlock, length);
}

MFSFSEntryMeta * MFSPartition::GetEntryMeta(uint32_t fsnodeId) const
{
    return &_fsnodePool[fsnodeId];
}


#define CBLOCK_STREAM_MAX_LEN        0xFFFFFFFFFFFFFFFF

MFSPartition::ChainedBlockStream::ChainedBlockStream(MFSPartition * partition, DWORD firstBlockId)
    : ChainedBlockStream(partition, firstBlockId, CBLOCK_STREAM_MAX_LEN)
{
    _length = GetLength();
}

MFSPartition::ChainedBlockStream::ChainedBlockStream(MFSPartition * partition, DWORD firstBlockId, UINT64 length)
    : MFSBlockStream(partition->_device.get()), _partition(partition), _firstBlock(firstBlockId),
      _currentBlock(firstBlockId), _blockOffset(0), _length(length)
{
    MFSBlockStream::Seek(MFSStreamSeekOrigin::Begin, GetDeviceBlockSize() * _firstBlock);
}

MFSPartition * MFSPartition::ChainedBlockStream::GetPartition() const
{
    return _partition;
}

bool MFSPartition::ChainedBlockStream::HasNext() const
{
    return _currentBlock != BLOCK_CHAIN_TAIL;
}

UINT64 MFSPartition::ChainedBlockStream::GetLength() const
{
    if (_length != CBLOCK_STREAM_MAX_LEN)
        return _length;
    else
    {
        UINT64 result = 0;
        DWORD blockSize = GetDeviceBlockSize();
        DWORD blockId = _firstBlock;
        while (blockId != BLOCK_CHAIN_TAIL)
        {
            result += blockSize;
            blockId = GetNextBlockId(blockId);
        }

        return result;
    }
}

UINT64 MFSPartition::ChainedBlockStream::GetPosition() const
{
    UINT64 result = 0;
    DWORD blockSize = GetDeviceBlockSize();
    DWORD blockId = _firstBlock;

    while (blockId != _currentBlock)
    {
        result += blockSize;
        blockId = GetNextBlockId(blockId);
    }

    result += _blockOffset;
    return result;
}

bool MFSPartition::ChainedBlockStream::Seek(MFSStreamSeekOrigin origin, INT64 offset)
{
    switch (origin)
    {
    case MFSStreamSeekOrigin::Begin:
        return SeekBegin(offset);
    case MFSStreamSeekOrigin::Relative:
        return SeekRelative(offset);
    case MFSStreamSeekOrigin::End:
        return SeekEnd(offset);
    default:
        return false;
    }
}

UINT64 MFSPartition::ChainedBlockStream::OnBlockSwap(UINT64 currentBlock)
{
    DWORD nextBlock = GetNextBlockId(static_cast<DWORD>(currentBlock));
    return nextBlock == BLOCK_CHAIN_TAIL 
        ? _partition->_device->GetBlocksCount() 
        : nextBlock;
}

DWORD MFSPartition::ChainedBlockStream::GetNextBlockId(DWORD current) const
{
    return _partition->_blockChain->Get(current);
}

DWORD MFSPartition::ChainedBlockStream::GetNextBlockId() const
{
    return GetNextBlockId(_currentBlock);
}

bool MFSPartition::ChainedBlockStream::SeekBegin(INT64 offset)
{
    if (offset < 0)
        return false;

    DWORD blockSize = GetDeviceBlockSize();
    DWORD blockId = _firstBlock;
    while (offset >= blockSize && blockId != BLOCK_CHAIN_TAIL)
    {
        offset -= blockSize;
        blockId = GetNextBlockId(blockId);
    }

    if (blockId == BLOCK_CHAIN_TAIL)
    {
        if (offset != 0)
            return false;
        else
        {
            _currentBlock = BLOCK_CHAIN_TAIL;
            _blockOffset = 0;
            return true;
        }
    }
    else
    {
        _currentBlock = blockId;
        _blockOffset = static_cast<DWORD>(offset);
        return true;
    }
}

bool MFSPartition::ChainedBlockStream::SeekRelative(INT64 offset)
{
    return SeekBegin(GetPosition() + offset);
}

bool MFSPartition::ChainedBlockStream::SeekEnd(INT64 offset)
{
    if (offset > 0)
        return false;
    else if (offset == 0)
    {
        _currentBlock = BLOCK_CHAIN_TAIL;
        _blockOffset = 0;
        return true;
    }
    else
    {
        return SeekBegin(GetLength() + offset);
    }
}
