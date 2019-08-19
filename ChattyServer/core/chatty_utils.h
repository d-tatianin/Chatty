#pragma once

#include <iostream>
#include <string>

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
	std::cout << "Usage: ChattyServer.exe <port>" << std::endl;
}

inline Chatty::endpoint get_endpoint(unsigned short port)
{
	return Chatty::endpoint(boost::asio::ip::tcp::v4(), port);
}
