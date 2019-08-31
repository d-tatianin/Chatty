#include "encoded_wide_string.h"
#include "wide_string.h"

#include "core/chatty_defines.h"

void wide_string::decode_from(const encoded_wide_string& encoded_str)
{
    m_Buffer.resize(encoded_str.size() + 1);

    for (size_t i = 0; i < encoded_str.size(); i++)
        m_Buffer[i] = static_cast<wchar_t>(encoded_str[i]);
}

void wide_string::decode_from(std::istream& byte_stream)
{
    m_Buffer.resize(CHATTY_MESSAGE_SIZE);

    char tempo[4];

    uint16_t chars_written = 0;

    while (byte_stream.readsome(tempo, 4) >= 4)
    {
        m_Buffer[chars_written] = static_cast<wchar_t>(*reinterpret_cast<uint32_t*>(tempo));

        ++chars_written;
    }

    m_Buffer.resize(chars_written);
}

void wide_string::from_wstring(const std::wstring& string)
{
    m_Buffer.reserve(string.size());

    for (auto c : string)
        m_Buffer.push_back(c);
}

wchar_t& wide_string::operator[](size_t index)
{
    return m_Buffer[index];
}

const wchar_t& wide_string::operator[](size_t index) const
{
    return m_Buffer[index];
}

size_t wide_string::size() const
{
    return m_Buffer.size();
}

std::wostream& operator<<(std::wostream& wide_stream, wide_string& string)
{
    wide_stream << string.m_Buffer.data();

    return wide_stream;
}