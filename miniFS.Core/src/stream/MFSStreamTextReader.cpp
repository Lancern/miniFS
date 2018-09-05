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

wchar_t MFSStreamTextReader::ReadChar()
{
    wchar_t ch;
    _stream->Read(&ch, sizeof(wchar_t), sizeof(wchar_t));
    return ch;
}

MFSString MFSStreamTextReader::Read(const std::vector<wchar_t> & terminators, bool includeTerminator)
{
    std::vector<wchar_t> buffer;
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
