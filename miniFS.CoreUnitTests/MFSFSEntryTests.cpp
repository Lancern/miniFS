#include "stdafx.h"

#include "../miniFS.Core/include/MFSString.h"
#include "../miniFS.Core/include/device/MFSOSFileDevice.h"
#include "../miniFS.Core/include/device/MFSBlockDevice.h"
#include "../miniFS.Core/include/fs/MFSPartition.h"

#include "CppUnitTest.h"
#include <memory>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(MFSFSEntryTest)
{

public:
    TEST_METHOD(RootDirectoryMetaLogger)
    {
        HANDLE hFile = MFSUnitTestOpenPartitionFileHandle(true);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            Assert::Fail();
            return;
        }
        
        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

        // Assert #1.
        Assert::IsTrue(partition->IsValidDevice());
        Logger::WriteMessage(L"OK: Assert #1 passed.");
        // Assert #2.
        Assert::IsFalse(partition->IsRaw());
        Logger::WriteMessage(L"OK: Assert #2 passed.");

        std::unique_ptr<MFSFSEntry> root(partition->GetRoot());

        // Assert #3.
        Assert::IsTrue(root->GetEntryType() == MFSFSEntryType::Directory);
        Logger::WriteMessage(L"OK: Assert #3 passed.");

        // Log meta information of root directory.
        Logger::WriteMessage(L"Root directory metadata in partition:");
        LogFSEntryMetadata(root.get());

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();

        CloseHandle(hFile);
    }

    TEST_METHOD(AddSubDirectoryTest)
    {
        HANDLE hFile = MFSUnitTestOpenPartitionFileHandle(true);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            Assert::Fail();
            return;
        }

        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

        // Assert #1.
        Assert::IsTrue(partition->IsValidDevice());
        Logger::WriteMessage(L"OK: Assert #1 passed.");

        // Assert #2.
        Assert::IsFalse(partition->IsRaw());
        Logger::WriteMessage(L"OK: Assert #2 passed.");

        std::unique_ptr<MFSFSEntry> root(partition->GetRoot());
        std::unique_ptr<MFSFSEntry> subEntry(root->AddSubEntry(L"lancern"));
        if (!subEntry)
        {
            Logger::WriteMessage(L"ERR: Adding sub-entry \"lancern\" to root directory failed.");
            Assert::Fail();
            return;
        }

        Logger::WriteMessage(L"Successfully added sub-entry \"lancern\" to root directory.");

        // Assert #3.
        Assert::IsTrue(root->ContainsSubEntry(L"lancern"));
        Logger::WriteMessage(L"OK: Assert #3 passed.");

        // Output metadata of the new entry.
        Logger::WriteMessage(L"Metadata of the new entry \"lancern\":");
        LogFSEntryMetadata(subEntry.get());

        subEntry.release();

        if (!root->RemoveSubEntry(L"lancern"))
        {
            Logger::WriteMessage(L"ERR: Removing sub-entry \"lancern\" from root directory failed.");
            Assert::Fail();
            return;
        }

        Logger::WriteMessage(L"Successfully removed sub-entry \"lancern\" from root directory.");

        // Assert #4.
        Assert::IsFalse(root->ContainsSubEntry(L"lancern"));
        Logger::WriteMessage(L"OK: Assert #4 passed.");

        root.release();
        partition.release();
        fileBlockDevice.release();
        fileDevice.release();

        CloseHandle(hFile);
    }

    TEST_METHOD(AddSubFileTest)
    {
        HANDLE hFile = MFSUnitTestOpenPartitionFileHandle(false);

    }

private:

    void LogFSEntryMetadata(const MFSFSEntry * entry)
    {
        Logger::WriteMessage(MFSString::Format(L"\tCreation time: %s",
            entry->GetCreationTime().GetDateTimeString().GetRawString()).GetRawString());
        Logger::WriteMessage(MFSString::Format(L"\tLast access time: %s",
            entry->GetCreationTime().GetDateTimeString().GetRawString()).GetRawString());
        Logger::WriteMessage(MFSString::Format(L"\tLast modification time: %s",
            entry->GetCreationTime().GetDateTimeString().GetRawString()).GetRawString());
        Logger::WriteMessage(MFSString::Format(L"\tNumber of children: %u",
            entry->GetSubEntriesCount()).GetRawString());
    }

};
