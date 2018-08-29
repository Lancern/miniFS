#pragma once

#include "../device/MFSBlockDevice.h"
#include "MFSMetas.h"

/*

class MFSPartition
��ʾһ���ļ�ϵͳ������

*/

class MFSPartition
{
public:
    MFSPartition(MFSBlockDevice * device);
    ~MFSPartition();

    MFSBlockDevice * GetDevice();
    const MFSBlockDevice * GetDevice() const;

    bool IsRaw() const;
    
    void BuildFileSystem();

private:
    MFSBlockDevice * _device;
};
