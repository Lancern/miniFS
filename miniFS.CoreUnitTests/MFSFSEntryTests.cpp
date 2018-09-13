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
            Assert::Fail(L"Failed to open partition file.");
        
        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

        // Assert #1.
        Assert::IsTrue(partition->IsValidDevice(), L"Invalid partition device.");

        // Assert #2.
        Assert::IsFalse(partition->IsRaw(), L"Raw partition device.");

        std::unique_ptr<MFSFSEntry> root(partition->GetRoot());

        // Assert #3.
        Assert::IsTrue(root->GetEntryType() == MFSFSEntryType::Directory, L"Unexpected root entry type.");

        // Log meta information of root directory.
        Logger::WriteMessage(L"Root directory metadata in partition:");
        LogFSEntryMetadata(root.get());

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();

        CloseHandle(hFile);
    }

    TEST_METHOD(AddSubEntryTest)
    {
        HANDLE hFile = MFSUnitTestOpenPartitionFileHandle(true);
        if (hFile == INVALID_HANDLE_VALUE)
            Assert::Fail(L"Failed to open partition file.");

        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

        // Assert #1.
        Assert::IsTrue(partition->IsValidDevice(), L"Invalid partition device.");

        // Assert #2.
        Assert::IsFalse(partition->IsRaw(), L"Row partition device.");

        std::unique_ptr<MFSFSEntry> root(partition->GetRoot());
        std::unique_ptr<MFSFSEntry> subEntry(root->AddSubEntry(L"lancern"));
        if (!subEntry)
            Assert::Fail(L"ERR: Adding sub-entry \"lancern\" to root directory failed.");

        Logger::WriteMessage(L"Successfully added sub-entry \"lancern\" to root directory.");
        subEntry->SetEntryType(MFSFSEntryType::Directory);

        // Assert #3.
        Assert::IsTrue(root->ContainsSubEntry(L"lancern"), L"Existence check failed after adding sub entry.");
        Assert::AreEqual<uint32_t>(root->GetSubEntriesCount(), 1, L"Unexpected subentries count.");

        // Output metadata of the new entry.
        Logger::WriteMessage(L"Metadata of the new entry \"lancern\":");
        LogFSEntryMetadata(subEntry.get());

        subEntry.reset(nullptr);

        if (!root->RemoveSubEntry(L"lancern"))
            Assert::Fail(L"ERR: Failed to remove sub-entry \"lancern\" from root directory failed.");

        Logger::WriteMessage(L"Successfully removed sub-entry \"lancern\" from root directory.");

        // Assert #4.
        Assert::IsFalse(root->ContainsSubEntry(L"lancern"), L"Existence check failed after removing sub entry.");

        root.reset(nullptr);
        partition.reset(nullptr);
        fileBlockDevice.reset(nullptr);
        fileDevice.reset(nullptr);

        CloseHandle(hFile);
    }

    TEST_METHOD(ResizeSubFileTest)
    {
        HANDLE hFile = MFSUnitTestOpenPartitionFileHandle(true);
        if (hFile == INVALID_HANDLE_VALUE)
            Assert::Fail(L"Failed to open partition file.");

        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

        // Assert #1.
        Assert::IsTrue(partition->IsValidDevice(), L"Invalid partition device.");

        // Assert #2.
        Assert::IsFalse(partition->IsRaw(), L"Raw partition device.");

        std::unique_ptr<MFSFSEntry> root(partition->GetRoot());
        if (!root)
            Assert::Fail(L"Failed to open fsentry for root directory.");
        
        std::unique_ptr<MFSFSEntry> fileEntry(root->AddSubEntry(L"lancern_file"));
        if (!fileEntry)
            Assert::Fail(L"Failed to add sub entry \"lancern_file\" to root directory.");

        fileEntry->SetEntryType(MFSFSEntryType::File);
        Assert::IsTrue(fileEntry->SetFileSize(4096 + 1), L"Failed to resize the file.");
        Assert::AreEqual<uint64_t>(fileEntry->GetFileSize(), 4096 + 1, L"Unexpected file size.");

        LogFSEntryMetadata(fileEntry.get());

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();

        CloseHandle(hFile);
    }

    TEST_METHOD(WriteFileTest)
    {
        HANDLE hFile = MFSUnitTestOpenPartitionFileHandle(true);
        if (hFile == INVALID_HANDLE_VALUE)
            Assert::Fail(L"Failed to open partition file.");

        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

        // Assert #1.
        Assert::IsTrue(partition->IsValidDevice(), L"Invalid partition device.");

        // Asert #2.
        Assert::IsFalse(partition->IsRaw(), L"Raw partition device.");

        std::unique_ptr<MFSFSEntry> root(partition->GetRoot());
        if (!root)
            Assert::Fail(L"Failed to open root directory.");

        std::unique_ptr<MFSFSEntry> subFile;
        if (root->ContainsSubEntry(L"lancern_file")) 
        {
            subFile.reset(root->GetSubEntry(L"lancern_file"));
            if (!subFile)
                Assert::Fail(L"Failed to open sub fsentry \"lancern_file\" from root directory.");
        }
        else
        {
            subFile.reset(root->AddSubEntry(L"lancern_file"));
            if (!subFile)
                Assert::Fail(L"Failed to add sub fsentry \"lancern_file\" to root directory.");

            subFile->SetEntryType(MFSFSEntryType::File);
            Assert::IsTrue(subFile->SetFileSize(4096 + 1), L"Failed to resize file.");
        }

        std::unique_ptr<MFSStream> fileStream(subFile->OpenDataStream());
        if (!fileStream)
            Assert::Fail(L"Failed to open file stream to write.");

        const wchar_t * buffer = L"My name is lancern.";
        uint32_t bufferSize = wcslen(buffer) * sizeof(wchar_t);
        uint32_t offset = 4097 - bufferSize;

        Assert::IsTrue(fileStream->Seek(MFSStreamSeekOrigin::Begin, offset), L"Failed to seek on file stream to write.");
        
        uint32_t written = fileStream->Write(buffer, bufferSize);
        Assert::AreEqual(bufferSize, written, L"Unexpeced number of bytes written to file stream.");

        fileStream->Flush();
        fileStream->Close();

        Logger::WriteMessage(L"Successfully write message to the file.");

        subFile.reset(root->GetSubEntry(L"lancern_file"));
        if (!subFile)
            Assert::Fail(L"Failed to reopen sub fsentry \"lancern_file\" from root directory.");
        
        fileStream.reset(subFile->OpenDataStream());
        if (!fileStream)
            Assert::Fail(L"Failed to open file stream to read.");

        Assert::IsTrue(fileStream->Seek(MFSStreamSeekOrigin::End, -static_cast<int32_t>(bufferSize)), 
            L"Failed to seek on file stream to read.");

        std::unique_ptr<uint8_t[]> readBuffer(new uint8_t[bufferSize]);
        memset(readBuffer.get(), 0, bufferSize);

        uint32_t read = fileStream->Read(readBuffer.get(), bufferSize, bufferSize);
        Assert::AreEqual(bufferSize, read, L"Unexpected number of bytes read from file stream.");

        Assert::IsTrue(memcmp(buffer, readBuffer.get(), bufferSize) == 0,
            L"Write buffer and read buffer not match.");

        fileStream->Close();

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();

        CloseHandle(hFile);
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

        if (entry->GetEntryType() == MFSFSEntryType::Directory)
        {
            Logger::WriteMessage(L"\tDirectory properties:");
            Logger::WriteMessage(MFSString::Format(L"\t\tNumber of children: %u",
                entry->GetSubEntriesCount()).GetRawString());
        }
        else
        {
            Logger::WriteMessage(L"\tFile properties:");
            Logger::WriteMessage(MFSString::Format(L"\t\tFile size: %llu bytes", entry->GetFileSize())
                .GetRawString());
        }
    }

};
