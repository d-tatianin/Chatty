#pragma once

#include <boost/asio.hpp>

#include "core/chatty_defines.h"
#include "core/chatty_types.h"

namespace Chatty {

	class client
	{
	private:
		socket         m_Socket;
		uint32_t       m_ID;
		std::string    m_Name;
		buffer         m_Buffer;
		mutable_buffer m_MutableBuffer;
		bool           m_ActiveSession;
		uint32_t       m_TalkingTo;
	public:
		client(socket&& client_socket, uint32_t id);
		void read();
		void on_read(const error_code& ec, size_t bytesTransferred);
		void reply(const error_code& ec);

		const std::string& name() { return m_Name; }
		void set_name(const std::string& name) { m_Name = name; }

		bool is_in_session() { return m_ActiveSession; }
		void enable_session() { m_ActiveSession = true; }
		void disable_session() { m_ActiveSession = false; }

		uint32_t chatter() { return m_TalkingTo; }
		void set_chatter(uint32_t id) { m_TalkingTo = id; }

		uint32_t id() { return m_ID; }
	};
}
