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
    std::vector<WCHAR> buffer;
    while (_stream->HasNext())
    {
        buffer.push_back(ReadPODObject<WCHAR>());
        if (buffer.back() == 0)
            break;
    }
    if (buffer.back() != 0)
        buffer.push_back(0);
    return MFSString(buffer.data(), buffer.size());
}
