#pragma once

#include <cstdint>
#include <vector>
#include <memory>

class MFSBlockAllocationBitmap
{
public:
	static constexpr uint32_t InvalidBlockId = ~0;

	MFSBlockAllocationBitmap(uint32_t sizeBits);
	~MFSBlockAllocationBitmap();

	uint32_t AllocBlock();
    bool AllocBlock(uint32_t blockId);
	bool FreeBlock(uint32_t pos);

    void Set(uint32_t pos);
    void Reset(uint32_t pos);

private:
	class Bitmap
	{
		static constexpr uint32_t BIT_PACK_SIZE = 64;

	public:
		class Reference
		{
		public:
			Reference(uint64_t & value, uint32_t offset);
			~Reference();

			Reference& operator = (bool x);
			Reference& operator = (const Reference& x);
			operator bool() const;

		private:
			uint64_t & value;
			uint32_t offset;
		};

		explicit Bitmap(uint32_t sizeInBits);
		~Bitmap();

		bool Test(uint32_t pos) const;
		uint32_t Size() const;
		void Set(uint32_t pos, bool value = true);
		void Reset(uint32_t pos);

		Reference operator [] (uint32_t pos);
		bool operator [] (uint32_t pos) const;

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
