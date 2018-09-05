#include "../../include/stream/MFSStreamWriter.h"

MFSStreamWriter::MFSStreamWriter(MFSStream * stream)
    : _stream(stream)
{
}

MFSStream * MFSStreamWriter::GetStream() const
{
    return _stream;
}

void MFSStreamWriter::Write(const MFSString & string)
{
    _stream->Write(string.GetRawString(), string.GetLength() * sizeof(wchar_t));
    this->Write<wchar_t>(0);
}
