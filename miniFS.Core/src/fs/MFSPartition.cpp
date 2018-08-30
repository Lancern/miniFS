#include "../../include/fs/MFSPartition.h"

MFSPartition::MFSPartition(MFSBlockDevice * device)
    : _device(device)
{
    // Load device.
    
}

MFSPartition::~MFSPartition()
{
    Close();
}

MFSBlockDevice * MFSPartition::GetDevice() const
{
    return _device;
}

bool MFSPartition::IsRaw() const
{
    // TODO: Implement MFSPartition::IsRaw().
    return false;
}

UINT64 MFSPartition::GetTotalSpaceInBytes() const
{
    // TODO: Implement MFSPartition::GetTotalSpaceInBytes().
    return UINT64();
}

UINT64 MFSPartition::GetFreeSpaceInBytes() const
{
    // TODO: Implement MFSPartition::GetFreeSpaceInBytes().
    return UINT64();
}

void MFSPartition::BuildFileSystem()
{
    // TODO: Implement MFSPartition::BuildFileSystem().
}

MFSFSEntry * MFSPartition::GetRoot() const
{
    // TODO: Implement MFSPartition::GetRoot().
    return nullptr;
}

void MFSPartition::Flush()
{
    // TODO: Implement MFSPartition::Flush().
}

void MFSPartition::Close()
{
    // TODO: Implement MFSPartition::Close().
}
