#include "wide_string.h"
#include "encoded_wide_string.h"

void encoded_wide_string::encode_from(const wide_string& plain_str)
{
    m_Buffer.resize(plain_str.size());

    for (size_t i = 0; i < plain_str.size(); i++)
        m_Buffer[i] = static_cast<uint32_t>(plain_str[i]);
}

uint32_t& encoded_wide_string::operator[](size_t index)
{
    return m_Buffer[index];
}

const uint32_t& encoded_wide_string::operator[](size_t index) const
{
    return m_Buffer[index];
}

size_t encoded_wide_string::size() const
{
    return m_Buffer.size();
}

void encoded_wide_string::copy_to(std::ostream& buffer)
{
    buffer.write(
        reinterpret_cast<char*>(m_Buffer.data()),
        m_Buffer.size() * 4
    );
}
