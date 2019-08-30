#pragma once

#include <istream>
#include <vector>
#include <string>

#include "core/chatty_types.h"

class encoded_wide_string;

class wide_string
{
private:
    std::vector<wchar_t> m_Buffer;
public:
    void decode_from(const encoded_wide_string& encoded_str);

    void decode_from(std::istream& byte_stream);

    void from_wstring(const std::wstring& string);

    wchar_t& operator[](size_t index);
    const wchar_t& operator[](size_t index) const;

    size_t size() const;

    friend std::wostream& operator<<(std::wostream& wide_stream, wide_string& string);
};
