#include "decoder.h"

namespace Chatty {

    decoder::decoder(const std::istream& stream)
    {
        std::stringstream ss;
        ss << stream.rdbuf();
        std::string packet = ss.str();
        
        if (isalnum(packet[0]))
        {
            m_PacketType = static_cast<packet_type::id>(packet[0] - '0');
        }
        else
        {
            m_PacketType = packet_type::BAD_PACKET;
            return;
        }

        packet.erase(0, 1);
        packet.push_back('\0');

        switch (m_PacketType)
        {
        case packet_type::USER_REGISTER:
        case packet_type::BEGIN_SESSION:
        case packet_type::CHAT_MESSAGE:
        case packet_type::CLOSE_SESSION:
            m_Message = std::vector<char>(packet.begin(), packet.end());
        default:
            break;
        }
    }
}