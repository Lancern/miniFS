#pragma once

#include "MFSRawDevice.h"
#include <vector>

/*

class MFSOSFileDevice
��ʾ�ɵײ����ϵͳ�ṩ�Ĵ����ļ��豸��

*/

class MFSOSFileDevice :
    public MFSRawDevice
{
public:
    MFSOSFileDevice(HANDLE hFile, bool readonly);
    ~MFSOSFileDevice();

    bool CanWrite() const override;
    UINT64 GetTotalSize() const override;

    MFSRawDeviceView * OpenView(UINT64 offset, DWORD viewSize);

    void Close() override;

private:
    HANDLE _hFile;
    HANDLE _hFileMapping;
    UINT64 _fileSize;
    bool _canWrite;
};

