#include "../../include/fs/MFSPartition.h"
#include "../../include/stream/MFSMemoryStream.h"
#include "../../include/serialize/MFSBlockAllocationBitmapSerializer.h"
#include "../../include/serialize/MFSFileAllocationTableSerializer.h"
#include "../../include/serialize/MFSFSNodePoolSerializer.h"
#include "../../include/stream/MFSStreamWriter.h"
#include "../../include/MFSDateTime.h"



#define CEIL_DIV(a, b)      ((a) / (b) + (a) % (b) ? 1 : 0)




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

uint64_t MFSPartition::GetTotalSpaceInBytes() const
{
    return static_cast<uint64_t>(_master.totalBlocks) * _device->GetBlockSize();
}

uint64_t MFSPartition::GetFreeSpaceInBytes() const
{
    return static_cast<uint64_t>(_master.freeBlocks) * _device->GetBlockSize();
}

void MFSPartition::BuildFileSystem()
{
    // Build partition master record.
    _master.magicSeq = MFS_MAGIC_SEQ;
    _master.mfsVer = MFS_VER;
    _master.totalBlocks = static_cast<uint32_t>(_device->GetBlocksCount());

    uint32_t blockSize = _device->GetBlockSize();
    uint32_t babBlocksCount = _master.totalBlocks / CHAR_BIT / blockSize;
    uint32_t fatBlocksCount = _master.totalBlocks * sizeof(uint32_t) / blockSize;
    uint32_t fsnodePoolBlocksCount = _master.totalBlocks * sizeof(MFSFSEntryMeta) / blockSize;
    uint32_t fsblocksCount = 1 + babBlocksCount + fatBlocksCount + fsnodePoolBlocksCount;    // mini-FS 占用的块数量

    _master.freeBlocks = _master.totalBlocks - fsblocksCount;

    // Build block allocation bitmap.
    _blockAllocation.reset(new MFSBlockAllocationBitmap(static_cast<uint32_t>(_device->GetBlocksCount())));
    for (uint32_t blockId = 0; blockId < fsblocksCount; ++blockId)
        _blockAllocation->Set(blockId);

    // Build file allocation table.
    _blockChain.reset(new MFSFileAllocationTable(static_cast<uint32_t>(_device->GetBlocksCount())));
    uint32_t babBlockOffset = 1;
    uint32_t fatBlockOffset = babBlockOffset + babBlocksCount;
    uint32_t fsnodePoolBlockOffset = fatBlockOffset + fatBlocksCount;

    _blockChain->Set(0, MFSFileAllocationTable::InvalidBlockId);

    for (uint32_t i = 0; i < babBlocksCount; ++i)
        _blockChain->Set(babBlockOffset + i, babBlockOffset + i + 1);
    _blockChain->Set(babBlockOffset + babBlocksCount - 1, MFSFileAllocationTable::InvalidBlockId);

    for (uint32_t i = 0; i < fatBlocksCount; ++i)
        _blockChain->Set(fatBlockOffset + i, fatBlockOffset + i + 1);
    _blockChain->Set(fatBlockOffset + fatBlocksCount - 1, MFSFileAllocationTable::InvalidBlockId);

    for (uint32_t i = 0; i < fsnodePoolBlocksCount; ++i)
        _blockChain->Set(fsnodePoolBlockOffset + i, fsnodePoolBlockOffset + i + 1);
    _blockChain->Set(fsnodePoolBlockOffset + fsnodePoolBlocksCount - 1, MFSFileAllocationTable::InvalidBlockId);

    // Initialize fsnode pool.
    _fsnodePool.reset(new MFSFSNodePool(static_cast<uint32_t>(_device->GetBlocksCount())));
    // Initialize the first fsnode as the node for the root directory.
    // Force allocate the first fsnode as the fsnode for the root directory.
    _fsnodePool->Allocate(0);
}

MFSFSEntry * MFSPartition::GetRoot()
{
    if (!_validDevice || IsRaw())
        return nullptr;

    MFSFSEntry * entry = new MFSFSEntry(GetInternalObject(), 0);
    return entry;
}

MFSPartition::Internals MFSPartition::GetInternalObject()
{
    return Internals(this);
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
    stream.Write(_fsnodePool.get(), 
        static_cast<uint32_t>(_device->GetBlocksCount() * sizeof(MFSFSEntryMeta)));

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
    uint32_t read = deviceStream->Read(buffer, 0, deviceStream->GetDeviceBlockSize());

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
    uint32_t dwBabByteSize = CEIL_DIV(deviceStream->GetDeviceBlockSize(), CHAR_BIT);
    void * buffer = VirtualAlloc(NULL, dwBabByteSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    uint32_t read = deviceStream->Read(buffer, dwBabByteSize, dwBabByteSize);
    bool ret = (read == dwBabByteSize);

    if (ret)
    {
        MFSMemoryStream memStream(buffer, dwBabByteSize);
        _blockAllocation.reset(MFSBlockAllocationBitmapSerializer().Deserialize(&memStream));
        memStream.Close();
    }

    VirtualFree(buffer, 0, MEM_RELEASE);

    if (!ret)
        _validDevice = false;

    return ret;
}

bool MFSPartition::LoadAllocationTable(MFSBlockStream * deviceStream)
{
    uint32_t dwFatByteSize = static_cast<uint32_t>(deviceStream->GetDeviceBlocksCount()) * sizeof(UINT32);
    void * buffer = VirtualAlloc(NULL, dwFatByteSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    uint32_t read = deviceStream->Read(buffer, dwFatByteSize, dwFatByteSize);
    bool ret = (read == dwFatByteSize);

    if (ret)
    {
        MFSMemoryStream memStream(buffer, dwFatByteSize);
        _blockChain.reset(MFSFileAllocationTableSerializer().Deserialize(&memStream));
        memStream.Close();
    }

    VirtualFree(buffer, 0, MEM_RELEASE);

    if (!ret)
        _validDevice = false;

    return ret;
}

bool MFSPartition::LoadFSNodePool(MFSBlockStream * deviceStream)
{
    uint32_t dwFsnodepoolByteSize = static_cast<uint32_t>(deviceStream->GetDeviceBlocksCount()) * sizeof(MFSFSEntryMeta);
    void * buffer = VirtualAlloc(NULL, dwFsnodepoolByteSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    uint32_t read = deviceStream->Read(buffer, dwFsnodepoolByteSize, dwFsnodepoolByteSize);
    bool ret = (read == dwFsnodepoolByteSize);

    if (ret)
    {
        MFSMemoryStream memStream(buffer, dwFsnodepoolByteSize);
        _fsnodePool.reset(MFSFSNodePoolSerializer().Deserialize(&memStream));
        memStream.Close();
    }

    VirtualFree(buffer, 0, MEM_RELEASE);

    if (!ret)
        _validDevice = false;

    return ret;
}
