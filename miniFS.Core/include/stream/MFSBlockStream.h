#pragma once

#include "MFSStream.h"
#include "../device/MFSBlockDevice.h"
#include <memory>


/*

class MFSBlockStream
��ʾ��һ�����豸��Ϊ�����豸��������

    ��Ա������

    bool SeekBlock(UINT64 blockId)
        �ƶ���ָ�뵽ָ���豸�����ʼλ�á�

    UINT64 GetCurrentBlockId() const
        ��ȡ��ǰ��ָ����λ�ڵĿ��š�

    UINT64 GetBlockInternalOffset() const
        ��ȡ��ǰ��ָ����λ�ڵĿ���ƫ������

    UINT64 OnBlockSwap(UINT64 currentBlock)
        �������齻��ʱ�����ú�����������������дʱ�����ݵ�ǰ����ȷ��Ҫ����Ŀ��š�

*/

class MFSBlockStream
    : public MFSStream
{
public:
    MFSBlockStream(MFSBlockDevice * device);
    
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;

    bool HasNext() const override;

    UINT64 GetLength() const override;
    UINT64 GetPosition() const override;

    MFSBlockDevice * GetDevice() const;
    DWORD GetDeviceBlockSize() const;
    UINT64 GetDeviceBlocksCount() const;

    DWORD Read(LPVOID lpBuffer, DWORD dwBufferSize, DWORD dwNumberOfBytesToRead) override;
    DWORD Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite) override;
    bool Seek(MFSStreamSeekOrigin origin, INT64 offset) override;

    void Flush() override;
    void Close() override;

protected:
    bool SeekBlock(UINT64 blockId);

    UINT64 GetCurrentBlockId() const;
    UINT64 GetBlockInternalOffset() const;

    virtual UINT64 OnBlockSwap(UINT64 currentBlock);

private:
    MFSBlockDevice * _device;
    std::unique_ptr<BYTE[]> _buffer;
    DWORD _insideOffset;
    UINT64 _blockOffset;

    bool SeekBegin(INT64 offset);

    bool TryReadByte(BYTE * buffer);
    bool TryWriteByte(BYTE data);
};
