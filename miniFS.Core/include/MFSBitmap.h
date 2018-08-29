#pragma once

#include <cstdint>
#include <vector>

class MFSBitmap
{
	static constexpr size_t BIT_PACK_SIZE = 64;

public:
	class Reference
	{
	public:
		Reference(uint64_t& value, size_t offset);
		~Reference();

		Reference& operator = (bool x);
		Reference& operator = (const Reference& x);
		operator bool() const;

	private:
		uint64_t& value;
		size_t offset;
	};

	explicit MFSBitmap(size_t size);
	~MFSBitmap();

	bool Test(size_t pos) const;
	size_t Size() const;
	void Set(size_t pos, bool value = true);
	void Reset(size_t pos);

    Reference operator [] (size_t pos);
    bool operator [] (size_t pos) const;

	friend class MFSBitmapSerializer;

private:
	std::vector<uint64_t> _bitmap;
};

