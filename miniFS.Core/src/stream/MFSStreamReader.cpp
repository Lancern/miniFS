#include "../../include/stream/MFSStreamReader.h"
#include <vector>
#include <algorithm>


MFSStreamReader::MFSStreamReader(MFSStream * stream)
    : _stream(stream)
{
}

MFSStream * MFSStreamReader::GetStream() const
{
    return _stream;
}

MFSString MFSStreamReader::ReadString()
{
    std::vector<wchar_t> buffer;
    while (_stream->HasNext())
    {
        buffer.push_back(ReadPODObject<wchar_t>());
        if (buffer.back() == 0)
            break;
    }
    if (buffer.back() == 0)
        buffer.pop_back();
    return MFSString(buffer.data(), buffer.size());
}
