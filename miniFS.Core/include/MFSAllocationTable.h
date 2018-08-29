#pragma once

#include <cstdint>
#include <vector>

class MFSAllocationTable
{
public:
	MFSAllocationTable(size_t size);
	~MFSAllocationTable();

	int32_t& operator [] (size_t pos);
	int32_t operator [] (size_t pos) const;
	void Add(int32_t first, int32_t last);

private:
	std::vector<int32_t> _next;
};
