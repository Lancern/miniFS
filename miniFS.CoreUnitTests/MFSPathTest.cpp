#include "stdafx.h"

#include "../miniFS.Core/dist/include/MFSPath.h"
#include "../miniFS.Core/dist/include/exceptions/MFSInvalidPathException.h"
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

		Assert::IsFalse(MFSPath::IsValidPath(L" "));

		Assert::IsFalse(MFSPath::IsValidPath(L"   "));

		Assert::IsFalse(MFSPath::IsValidPath(L"/ "));

		Assert::IsFalse(MFSPath::IsValidPath(L" /"));

		Assert::IsFalse(MFSPath::IsValidPath(L"a /"));

		Assert::IsFalse(MFSPath::IsValidPath(L" a/"));

		Assert::IsFalse(MFSPath::IsValidPath(L""));

		Assert::IsTrue(MFSPath::IsValidPath(L"a b/"));

		Assert::IsTrue(MFSPath::IsValidPath(L"/a b"));

		Assert::IsTrue(MFSPath::IsValidPath(L"/.a"));

		Assert::IsTrue(MFSPath::IsValidPath(L".a"));

		Assert::IsTrue(MFSPath::IsValidPath(L"/..a"));

		Assert::IsTrue(MFSPath::IsValidPath(L"..a"));

		Assert::IsTrue(MFSPath::IsValidPath(L"/"));

		Assert::IsTrue(MFSPath::IsValidPath(L"./"));

		Assert::IsTrue(MFSPath::IsValidPath(L"/."));

		Assert::IsTrue(MFSPath::IsValidPath(L"/.."));

		Assert::IsTrue(MFSPath::IsValidPath(L"/./"));

		Assert::IsTrue(MFSPath::IsValidPath(L"/../.."));

		Assert::IsTrue(MFSPath::IsValidPath(L"."));

		Assert::IsTrue(MFSPath::IsValidPath(L".."));

		Assert::IsTrue(MFSPath::IsValidPath(L"..."));

		Assert::IsTrue(MFSPath::IsValidPath(L"\\"));

		Assert::IsTrue(MFSPath::IsValidPath(L"D\\"));

		Assert::IsTrue(MFSPath::IsValidPath(L"D.\\"));

		Assert::IsTrue(MFSPath::IsValidPath(L"\\D:"));
	}

	TEST_METHOD(IsAbsolutePathTest)
	{
		Assert::IsFalse(MFSPath::IsAbsolutePath(L"//"));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L"D:\\"));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L"./"));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L"../"));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L" /"));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L"/ "));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L"  /"));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L"a/"));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L"\\"));

		Assert::IsFalse(MFSPath::IsAbsolutePath(L"\\/"));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/"));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/."));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/.."));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/..."));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/../.."));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/a"));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/a b"));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/a  b"));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/a b/"));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/\\/"));

		Assert::IsTrue(MFSPath::IsAbsolutePath(L"/a b/Ä²ÉñÌ«Ç¿À±"));
	}

	TEST_METHOD(IsOSPathTest)
	{
		Assert::IsFalse(MFSPath::IsOSPath(L" "));

		Assert::IsFalse(MFSPath::IsOSPath(L"/."));

		Assert::IsFalse(MFSPath::IsOSPath(L"."));

		Assert::IsFalse(MFSPath::IsOSPath(L"C:/"));

		Assert::IsFalse(MFSPath::IsOSPath(L" C:\\"));

		Assert::IsFalse(MFSPath::IsOSPath(L"1:\\"));

		Assert::IsFalse(MFSPath::IsOSPath(L"Ä²:\\"));

		Assert::IsTrue(MFSPath::IsOSPath(L"d:\\"));

		Assert::IsTrue(MFSPath::IsOSPath(L"D:\\"));
	}

	TEST_METHOD(GetPathNamesTest)
	{
		Assert::ExpectException<MFSInvalidPathException>([]()
		{
			MFSPath::GetPathNames(L" ");
		});

		Assert::ExpectException<MFSInvalidPathException>([]()
		{
			MFSPath::GetPathNames(L"D:\\");
		});

		Assert::ExpectException<MFSInvalidPathException>([]()
		{
			MFSPath::GetPathNames(L". a/b ");
		});

		Assert::IsTrue(MFSPath::GetPathNames(L"./.") == std::vector<MFSString>{L".", L"."});

		Assert::IsTrue(MFSPath::GetPathNames(L"/a b/../") == std::vector<MFSString>{L"a b", L".."});

		Assert::IsTrue(MFSPath::GetPathNames(L"../..") == std::vector<MFSString>{L"..", L".."});

		Assert::IsTrue(MFSPath::GetPathNames(L"../..") == std::vector<MFSString>{L"..", L".."});

		Assert::IsTrue(MFSPath::GetPathNames(L"ï¿½ï¿½/ÌÌÌÌÌÌ/ÍÍÍÍÍÍ/ï»ï»ï»") == std::vector<MFSString>{L"ï¿½ï¿½", L"ÌÌÌÌÌÌ", L"ÍÍÍÍÍÍ", L"ï»ï»ï»"});
	}

	TEST_METHOD(GetFileNameTest)
	{
		Assert::ExpectException<MFSInvalidPathException>([]()
		{
			MFSPath::GetFileName(L" ");
		});

		Assert::IsTrue(MFSPath::GetFileName(L"/") == L"");

		Assert::IsTrue(MFSPath::GetFileName(L"/\\/") == L"");

		Assert::IsTrue(MFSPath::GetFileName(L".") == L".");

		Assert::IsTrue(MFSPath::GetFileName(L"a") == L"a");

		Assert::IsTrue(MFSPath::GetFileName(L"./.") == L".");

		Assert::IsTrue(MFSPath::GetFileName(L"/a b") == L"a b");

		Assert::IsTrue(MFSPath::GetFileName(L"./a b/") == L"");

		Assert::IsTrue(MFSPath::GetFileName(L"ï¿½ï¿½ÌÌÌÌÌÌÍÍÍÍÍÍï»ï»ï»/") == L"");
	}

	TEST_METHOD(GetExtensionTest)
	{
		Assert::ExpectException<MFSInvalidPathException>([]()
		{
			MFSPath::GetExtension(L" ");
		});

		Assert::IsTrue(MFSPath::GetExtension(L".") == L"");

		Assert::IsTrue(MFSPath::GetExtension(L"..") == L"");

		Assert::IsTrue(MFSPath::GetExtension(L"a.") == L"");

		Assert::IsTrue(MFSPath::GetExtension(L"a..") == L"");

		Assert::IsTrue(MFSPath::GetExtension(L"a.ï¿½ï¿½") == L".ï¿½ï¿½");

		Assert::IsTrue(MFSPath::GetExtension(L".ï¿½ï¿½") == L".ï¿½ï¿½");

		Assert::IsTrue(MFSPath::GetExtension(L"/.ï¿½ï¿½") == L".ï¿½ï¿½");

		Assert::IsTrue(MFSPath::GetExtension(L".ï¿½ï¿½/") == L"");

		Assert::IsTrue(MFSPath::GetExtension(L"..ï¿½ï¿½") == L".ï¿½ï¿½");

		Assert::IsTrue(MFSPath::GetExtension(L"a.b.c") == L".c");

		Assert::IsTrue(MFSPath::GetExtension(L"a.b.c.") == L"");

		Assert::IsTrue(MFSPath::GetExtension(L"a.b c") == L".b c");
	}
};