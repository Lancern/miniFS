#include "../../include/stream/MFSStreamTextReader.h"
#include <algorithm>


MFSStreamTextReader::MFSStreamTextReader(MFSStream * stream)
    : _stream(stream)
{
}

MFSStream * MFSStreamTextReader::GetStream() const
{
    return _stream;
}

WCHAR MFSStreamTextReader::ReadChar()
{
    WCHAR ch;
    _stream->Read(&ch, sizeof(WCHAR), sizeof(WCHAR));
    return ch;
}

MFSString MFSStreamTextReader::Read(const std::vector<WCHAR>& terminators, bool includeTerminator)
{
    std::vector<WCHAR> buffer;
    while (_stream->HasNext())
    {
        buffer.push_back(ReadChar());
        if (std::find(terminators.begin(), terminators.end(), buffer.back()) != terminators.end())
        {
            if (!includeTerminator)
                buffer.pop_back();
            break;
        }
    }
    return MFSString(buffer.data(), buffer.size());
}

MFSString MFSStreamTextReader::ReadToken()
{
    return Read({ L' ', L'\n', L'\t', L'\r' }, false);
}

MFSString MFSStreamTextReader::ReadLine()
{
    return Read({ L'\n' }, true);
}
