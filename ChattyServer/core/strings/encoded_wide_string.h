#pragma once

#include <istream>
#include <vector>
#include <string>

#include "core/chatty_types.h"

class wide_string;

class encoded_wide_string
{
private:
    std::vector<uint32_t> m_Buffer;
public:
    void encode_from(const wide_string& plain_str);

    uint32_t& operator[](size_t index);
    const uint32_t& operator[](size_t index) const;

    size_t size() const;

    void copy_to(std::ostream& buffer);
};