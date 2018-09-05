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
            Assert::Fail();
            return;
        }

        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

        // ASSERT #1.
        Assert::IsFalse(partition->IsValidDevice());
        Logger::WriteMessage(L"OK: Assert #1 passed.");

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();
        partition.release();
        fileBlockDevice.release();
        fileDevice.release();

        LONG tmpFileSize = 128 * 1024 * 1024;
        if (SetFilePointer(hFile, tmpFileSize, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            MFSUnitTestLogLastError(L"SetFilePointer #1 failed");
            CloseHandle(hFile);
            Assert::Fail();

            return;
        }
        if (!SetEndOfFile(hFile))
        {
            MFSUnitTestLogLastError(L"SetEndOfFile failed");
            CloseHandle(hFile);
            Assert::Fail();

            return;
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            MFSUnitTestLogLastError(L"SetFilePointer #2 failed");
            CloseHandle(hFile);
            Assert::Fail();

            return;
        }

        fileDevice.reset(new MFSOSFileDevice(hFile, false));
        fileBlockDevice.reset(new MFSBlockDevice(fileDevice.get()));
        partition.reset(new MFSPartition(fileBlockDevice.get()));

        // ASSERT #2.
        Assert::IsTrue(partition->IsValidDevice());
        Logger::WriteMessage(L"OK: Assert #2 passed.");

        // ASSERT #3.
        Assert::IsTrue(partition->IsRaw());
        Logger::WriteMessage(L"OK: Assert #3 passed.");

        partition->BuildFileSystem();

        // ASSERT #4.
        Assert::IsFalse(partition->IsRaw());
        Logger::WriteMessage(L"OK: Assert #4 passed.");

        // ASSERT #5.
        Assert::AreEqual<uint64_t>(partition->GetTotalSpaceInBytes(), tmpFileSize);
        Logger::WriteMessage(L"OK: Assert #5 passed.");

        DWORD babBlocks = static_cast<DWORD>(
            fileBlockDevice->GetBlocksCount() / CHAR_BIT / fileBlockDevice->GetBlockSize());
        DWORD fatBlocks = static_cast<DWORD>(
            fileBlockDevice->GetBlocksCount() * sizeof(uint32_t) / fileBlockDevice->GetBlockSize());
        DWORD fsnpBlocks = static_cast<DWORD>(
            fileBlockDevice->GetBlocksCount() * sizeof(MFSFSEntryMeta) / fileBlockDevice->GetBlockSize());
        // ASSERT #6.
        Assert::AreEqual<uint64_t>(partition->GetFreeSpaceInBytes(), 
            (fileBlockDevice->GetBlocksCount() - 1 - babBlocks - fatBlocks - fsnpBlocks) * fileBlockDevice->GetBlockSize());
        Logger::WriteMessage(L"OK: Assert #6 passed.");

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();
        partition.release();
        fileBlockDevice.release();
        fileDevice.release();

        CloseHandle(hFile);
	}

};