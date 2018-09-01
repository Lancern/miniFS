#pragma once

#include <cstdint>
#include <vector>

class MFSFileAllocationTable
{
public:
	MFSFileAllocationTable(size_t size);
	~MFSFileAllocationTable();

	uint32_t& operator [] (size_t pos);
	uint32_t operator [] (size_t pos) const;

	void Add(uint32_t first, uint32_t last);
	size_t Size() const;

    uint32_t Get(size_t pos) const;
    void Set(size_t pos, size_t value);

	friend class MFSFileAllocationTableSerializer;

private:
	std::vector<uint32_t> _next;
};
