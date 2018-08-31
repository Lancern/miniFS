#include "../../include/stream/MFSStreamTextWriter.h"

MFSStreamTextWriter::MFSStreamTextWriter(MFSStream * stream)
    : _stream(stream)
{
}

MFSStream * MFSStreamTextWriter::GetStream() const
{
    return _stream;
}

void MFSStreamTextWriter::Write(const MFSString & string)
{
    _stream->Write(string.GetRawString(), string.GetLength() * sizeof(WCHAR));
}

void MFSStreamTextWriter::WriteChar(WCHAR ch)
{
    _stream->Write(&ch, sizeof(ch));
}

void MFSStreamTextWriter::WriteLine(const MFSString & string)
{
    Write(string);
    WriteChar(L'\n');
}
