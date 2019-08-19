#include "decoder.h"

namespace Chatty {

	decoder::decoder(const std::istream& stream)
	{
		std::stringstream ss;
		ss << &stream;
		std::string packet = ss.str();
		
		if (isalnum(packet[0]))
		{
			m_PacketType = (packet_type::id)(packet[0] - '0');
		}
		else
		{
			m_PacketType = packet_type::BAD_PACKET;
			return;
		}

		packet.erase(0, 1);

		switch (m_PacketType)
		{
		case packet_type::BEGIN_SESSION:
			m_Message = std::vector<char>(packet.begin(), packet.end());
		case packet_type::CHAT_MESSAGE:
			m_Message = std::vector<char>(packet.begin(), packet.end());
		default:
			break;
		}
	}

}