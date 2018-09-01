#pragma once

#include <cstdint>
#include <vector>
#include <memory>

class MFSBlockAllocationBitmap
{
public:
	static constexpr uint32_t InvalidBlockId = ~0;

	MFSBlockAllocationBitmap(size_t sizeBits);
	~MFSBlockAllocationBitmap();

	uint32_t AllocBlock();
	void FreeBlock(uint32_t pos);

    void Set(uint32_t pos);
    void Reset(uint32_t pos);

private:
	class Bitmap
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
			uint64_t & value;
			size_t offset;
		};

		explicit Bitmap(size_t sizeInBits);
		~Bitmap();

		bool Test(size_t pos) const;
		size_t Size() const;
		void Set(size_t pos, bool value = true);
		void Reset(size_t pos);

		Reference operator [] (size_t pos);
		bool operator [] (size_t pos) const;

		friend class MFSBitmapSerializer;

	private:
		std::vector<uint64_t> _bitmap;

		friend class MFSBlockAllocationBitmapSerializer;
	};

	std::shared_ptr<Bitmap> _bitmap;
	uint32_t _alloc;

    void LocateNextFreeBlock();

	friend class MFSBlockAllocationBitmapSerializer;
};
