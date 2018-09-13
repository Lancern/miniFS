#include "stdafx.h"

#include "../miniFS.Core/include/fs/MFSPartition.h"
#include "../miniFS.Core/include/device/MFSOSFileDevice.h"
#include "../miniFS.Core/include/device/MFSBlockDevice.h"

#include "CppUnitTest.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MFSPartitionTests)
{
public:
		
	TEST_METHOD(FormatTest)
	{
        HANDLE hFile = MFSUnitTestOpenPartitionFileHandle(false);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            Assert::Fail(L"Failed to open partition file.");
            return;
        }

        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

        // ASSERT #1.
        Assert::IsFalse(partition->IsValidDevice(), L"Invalid partition device.");

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();
        partition.reset(nullptr);
        fileBlockDevice.reset(nullptr);
        fileDevice.reset(nullptr);

        LONG tmpFileSize = 128 * 1024 * 1024;
        if (SetFilePointer(hFile, tmpFileSize, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            CloseHandle(hFile);
            Assert::Fail(L"Failed to set file pointer to 128 MB.");
            return;
        }
        if (!SetEndOfFile(hFile))
        {
            CloseHandle(hFile);
            Assert::Fail(L"Failed to set end of file.");

            return;
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            CloseHandle(hFile);
            Assert::Fail(L"Failed to set file pointer to start.");

            return;
        }

        fileDevice.reset(new MFSOSFileDevice(hFile, false));
        fileBlockDevice.reset(new MFSBlockDevice(fileDevice.get()));
        partition.reset(new MFSPartition(fileBlockDevice.get()));

        // ASSERT #2.
        Assert::IsTrue(partition->IsValidDevice(), L"Invalid partition device.");

        // ASSERT #3.
        Assert::IsTrue(partition->IsRaw(), L"Partition device is not raw.");

        partition->BuildFileSystem();

        // ASSERT #4.
        Assert::IsFalse(partition->IsRaw(), L"Raw partition device.");

        // ASSERT #5.
        Assert::AreEqual<uint64_t>(partition->GetTotalSpaceInBytes(), tmpFileSize, L"Unexpected partition total size.");

        DWORD babBlocks = static_cast<DWORD>(
            fileBlockDevice->GetBlocksCount() / CHAR_BIT / fileBlockDevice->GetBlockSize());
        DWORD fatBlocks = static_cast<DWORD>(
            fileBlockDevice->GetBlocksCount() * sizeof(uint32_t) / fileBlockDevice->GetBlockSize());
        DWORD fsnpBlocks = static_cast<DWORD>(
            fileBlockDevice->GetBlocksCount() * sizeof(MFSFSEntryMeta) / fileBlockDevice->GetBlockSize());
        // ASSERT #6.
        Assert::AreEqual<uint64_t>(partition->GetFreeSpaceInBytes(), 
            (fileBlockDevice->GetBlocksCount() - 1 - babBlocks - fatBlocks - fsnpBlocks) * fileBlockDevice->GetBlockSize(),
            L"Unexpected partition free size.");

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();
        partition.reset(nullptr);
        fileBlockDevice.reset(nullptr);
        fileDevice.reset(nullptr);

        CloseHandle(hFile);
	}

};