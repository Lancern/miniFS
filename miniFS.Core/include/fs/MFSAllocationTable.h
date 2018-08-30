#pragma once

#include <cstdint>
#include <vector>

class MFSAllocationTable
{
public:
	MFSAllocationTable(size_t size);
	~MFSAllocationTable();

	uint32_t& operator [] (size_t pos);
	uint32_t operator [] (size_t pos) const;
	void Add(uint32_t first, uint32_t last);
	size_t Size() const;

	friend class MFSAllocationTableSerializer;

private:
	std::vector<uint32_t> _next;
};
