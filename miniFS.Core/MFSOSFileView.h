#pragma once

#include "MFSRawDeviceView.h"

/*

class MFSOSFileView
��ʾ�ɵײ����ϵͳ�ṩ�Ĵ����ļ�����ķֲ���ͼ��

*/

class MFSOSFileView :
    public MFSRawDeviceView
{
public:
    ~MFSOSFileView();

    bool CanWrite() const override;
    DWORD GetSize() const override;

    DWORD Read(LPVOID lpBuffer, DWORD offset, DWORD length) override;
    DWORD Write(DWORD offset, DWORD length, LPCVOID lpBuffer) override;

    void Flush() override;
    void Close() override;

    friend class MFSOSFileDevice;

private:
    MFSOSFileView(HANDLE hFileMapping, UINT64 offset, DWORD size, bool readonly);

    LPVOID _lpFileMappingAddress;
    DWORD _viewSize;
    bool _canWrite;
};

