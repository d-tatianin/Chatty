#pragma once

#include <boost/asio.hpp>

#include "core/chatty_types.h"

namespace Chatty {

	class client
	{
	private:
		socket      m_Socket;
		uint32_t    m_ID;
		std::string m_Name;
	public:
		client(socket&& client_socket);
	};
}
