#include "stdafx.h"

#include "../miniFS.Core/dist/include/MFSPath.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MFSPathTest)
{
public:
	TEST_METHOD(IsValidPathTest)
	{
		Assert::IsFalse(MFSPath::IsValidPath(L"D:\\"));

		Assert::IsFalse(MFSPath::IsValidPath(L"/\""));

		Assert::IsFalse(MFSPath::IsValidPath(L"/\'"));

		Assert::IsFalse(MFSPath::IsValidPath(L"//"));

		Assert::IsFalse(MFSPath::IsValidPath(L".//."));

		Assert::IsFalse(MFSPath::IsValidPath(L".//."));

		Assert::IsTrue(MFSPath::IsValidPath(L"/"));

		Assert::IsTrue(MFSPath::IsValidPath(L"./"));

		Assert::IsTrue(MFSPath::IsValidPath(L"/."));

		Assert::IsTrue(MFSPath::IsValidPath(L"/.."));

		Assert::IsTrue(MFSPath::IsValidPath(L"/./"));

		Assert::IsTrue(MFSPath::IsValidPath(L"/../.."));

		Assert::IsTrue(MFSPath::IsValidPath(L"."));

		Assert::IsTrue(MFSPath::IsValidPath(L".."));

		Assert::IsTrue(MFSPath::IsValidPath(L"..."));
	}
};