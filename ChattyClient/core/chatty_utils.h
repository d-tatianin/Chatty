#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "chatty_types.h"

inline bool is_help(const std::string& testStr)
{
    if (testStr == "--help")
        return true;
    else if (testStr == "-help")
        return true;
    else if (testStr == "help")
        return true;
    else if (testStr == "Help")
        return true;
    else if (testStr == "-h")
        return true;
    else if (testStr == "?")
        return true;
    else
        return false;
}

inline void print_help()
{
    std::cout << "Usage: ChattyClient.exe <ip_address:port>" << std::endl;
}

bool extract_ip_port(char* ip_port, Chatty::string_pair& out_pair)
{
    std::vector<std::string> data;

    boost::split(data, ip_port, boost::is_any_of(":"));

    if (data.size() != 2)
    {
        out_pair.first = "Unknown";
        out_pair.second = "Unknown";

        return false;
    }
    else
    {
        out_pair.first = std::move(data[0]);
        out_pair.second = std::move(data[1]);

        return true;
    }
}
