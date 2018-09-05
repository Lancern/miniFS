#include "stdafx.h"

#include "../miniFS.Core/include/fs/MFSPartition.h"
#include "../miniFS.Core/include/device/MFSOSFileDevice.h"
#include "../miniFS.Core/include/device/MFSBlockDevice.h"

#include "CppUnitTest.h"
#include <Windows.h>
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MFSPartitionTests)
{
public:
		
	TEST_METHOD(FormatTest)
	{
        HANDLE hFile = CreateFileW(
            L"MFSPartitionFormatTest.dat",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_DELETE,
            NULL,
            CREATE_ALWAYS,
            // FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            LogLastError(L"CreateFileW failed");
            Assert::Fail();

            return;
        }

        std::unique_ptr<MFSOSFileDevice> fileDevice(new MFSOSFileDevice(hFile, false));
        std::unique_ptr<MFSBlockDevice> fileBlockDevice(new MFSBlockDevice(fileDevice.get()));
        std::unique_ptr<MFSPartition> partition(new MFSPartition(fileBlockDevice.get()));

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
            LogLastError(L"SetFilePointer #1 failed");
            CloseHandle(hFile);
            Assert::Fail();

            return;
        }
        if (!SetEndOfFile(hFile))
        {
            LogLastError(L"SetEndOfFile failed");
            CloseHandle(hFile);
            Assert::Fail();

            return;
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            LogLastError(L"SetFilePointer #2 failed");
            CloseHandle(hFile);
            Assert::Fail();

            return;
        }

        fileDevice.reset(new MFSOSFileDevice(hFile, false));
        fileBlockDevice.reset(new MFSBlockDevice(fileDevice.get()));
        partition.reset(new MFSPartition(fileBlockDevice.get()));

        Assert::IsTrue(partition->IsValidDevice());
        Logger::WriteMessage(L"OK: Assert #2 passed.");
        Assert::IsTrue(partition->IsRaw());
        Logger::WriteMessage(L"OK: Assert #3 passed.");

        partition->BuildFileSystem();

        Assert::IsFalse(partition->IsRaw());
        Logger::WriteMessage(L"OK: Assert #4 passed.");

        partition->Close();
        fileBlockDevice->Close();
        fileDevice->Close();
        partition.release();
        fileBlockDevice.release();
        fileDevice.release();

        CloseHandle(hFile);
	}

private:

    void LogLastError(const wchar_t * msg)
    {
        DWORD errCode = GetLastError();

        DWORD strBufferSize = MAX_PATH + 10;
        std::unique_ptr<WCHAR[]> strBuffer(new WCHAR[strBufferSize]);

        swprintf_s(strBuffer.get(),
            strBufferSize,
            L"ERR: %s. GetLastError() = %u: ",
            msg,
            static_cast<uint32_t>(errCode));
        Logger::WriteMessage(strBuffer.get());

        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
            errCode,
            0,
            strBuffer.get(),
            strBufferSize,
            NULL);
        Logger::WriteMessage(strBuffer.get());
    }

};