#pragma once

#include <vector>
#include <sstream>

namespace Chatty {

	namespace packet_type {

		enum id
		{
			BAD_PACKET    = -1,
			USER_REGISTER =  0, // [0] [USER_NAME]
			BEGIN_SESSION =  2, // [2] [OTHER_USER]
			CHAT_MESSAGE  =  3, // [3] [MESSAGE]
			CLOSE_SESSION =  4, // [4]
			OK_REPLY      =  5  // [5]
		};
	}
	
	class decoder
	{
	private:
		packet_type::id   m_PacketType;
		std::vector<char> m_Message;
	public:
		decoder(const std::istream& response_stream);
		packet_type::id    packet_type() { return m_PacketType; }
		std::vector<char>& message()     { return m_Message; }
	};
}