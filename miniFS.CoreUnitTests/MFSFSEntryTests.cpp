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
    TEST_METHOD(RootDirectoryMetaLogger)
    {
        HANDLE hFile = MFSUnitTestOpenPartitionFileHandle(true);
        
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
        Logger::WriteMessage(MFSString::Format(L"\tCreation time: %s",
            root->GetCreationTime().GetDateTimeString().GetRawString()).GetRawString());
        Logger::WriteMessage(MFSString::Format(L"\tLast access time: %s",
            root->GetCreationTime().GetDateTimeString().GetRawString()).GetRawString());
        Logger::WriteMessage(MFSString::Format(L"\tLast modification time: %s",
            root->GetCreationTime().GetDateTimeString().GetRawString()).GetRawString());
        Logger::WriteMessage(MFSString::Format(L"\tNumber of children: %u",
            root->GetSubEntriesCount()).GetRawString());

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();

        CloseHandle(hFile);
    }
};
