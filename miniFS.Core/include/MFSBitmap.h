#pragma once

#include <cstdint>
#include <vector>

class MFSBitmap
{
public:
	class Reference
	{
	public:
		Reference(std::vector<bool>::reference value);
		~Reference();

		Reference& operator = (bool x);
		Reference& operator = (const Reference& x);
		operator bool() const;

	private:
		std::vector<bool>::reference value;
	};

	explicit MFSBitmap(size_t size);
	~MFSBitmap();

	Reference operator [] (size_t pos);
	bool operator [] (size_t pos) const;
	bool Test(size_t pos) const;
	size_t Size() const;
	void Set(size_t pos, bool value = true);
	void Reset(size_t pos);
	void Read(std::vector<void*>& blocks, size_t blockSize);
	void Write(std::vector<void*>& blocks, size_t blockSize) const;

private:
	std::vector<bool> _bitmap;
};

