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

MFSString MFSStreamReader::ReadString(const std::vector<WCHAR> & terminators)
{
    std::vector<WCHAR> buffer;
    while (_stream->HasNext())
    {
        buffer.push_back(ReadPODObject<WCHAR>());
        if (std::find(terminators.begin(), terminators.end(), buffer.back()) != terminators.end())
            break;
    }
    return MFSString(buffer.data(), buffer.size());
}

MFSString MFSStreamReader::ReadString()
{
    return ReadString({});
}

MFSString MFSStreamReader::ReadLine()
{
    return ReadString({ L'\n' });
}
