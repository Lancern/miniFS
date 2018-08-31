#include "../../include/fs/MFSPartition.h"
#include "../../include/stream/MFSMemoryStream.h"
#include "../../include/serialize/MFSBitmapSerializer.h"
#include "../../include/serialize/MFSAllocationTableSerializer.h"
#include "../../include/stream/MFSStreamWriter.h"



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
    _master.totalBlocks = _device->GetBlocksCount();

    DWORD blockSize = _device->GetBlockSize();
    DWORD babBlocksCount = _master.totalBlocks / CHAR_BIT / blockSize;
    DWORD fatBlocksCount = _master.totalBlocks * sizeof(DWORD) / blockSize;;
    DWORD fsnodePoolBlockCount = _master.totalBlocks * sizeof(MFSFSEntryMeta) / blockSize;

    _master.freeBlocks = _master.totalBlocks - 1 - babBlocksCount - fatBlocksCount - fsnodePoolBlockCount;

    // Build block allocation bitmap.
    
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
    MFSBitmapSerializer().Serialize(&stream, _blockAllocation->GetBitmap());

    // Write block chain (FAT).
    MFSFileAllocationTableSerializer().Serialize(&stream, _blockChain.get());

    // Write fsnode pool.
    stream.Write(_fsnodePool.get(), _device->GetBlocksCount() * sizeof(MFSFSEntryMeta));

    stream.Flush();
    stream.Close();
}

void MFSPartition::Close()
{
    Flush();
    Close();
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
        _blockAllocation.reset(new MFSBlockAllocationBitmap(MFSBitmapSerializer().Deserialize(&memStream)));
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
    MFSFSEntryMeta * buffer = new MFSFSEntryMeta[deviceStream->GetDeviceBlocksCount()];

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
