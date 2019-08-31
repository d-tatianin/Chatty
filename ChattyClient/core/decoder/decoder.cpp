#include "decoder.h"

namespace Chatty {

    decoder::decoder(std::istream& stream)
    {
        m_PacketType = static_cast<packet_type::id>(stream.get());

        if (!validate(m_PacketType))
        {
            m_PacketType = packet_type::BAD_PACKET;
            return;
        }

        switch (m_PacketType)
        {
        case packet_type::USER_REGISTER:
        case packet_type::BEGIN_SESSION:
        case packet_type::CHAT_MESSAGE:
        case packet_type::CLOSE_SESSION:
            m_Message.decode_from(stream);
        default:
            break;
        }
    }

    void decoder::construct_packet(
        buffer& out_buffer,
        packet_type::id type,
        wide_string* message
    )
    {
        out_buffer.consume(out_buffer.size() + 1);

        std::ostream writer(&out_buffer);

        writer << static_cast<char>(type);

        if (message)
        {
            encoded_wide_string ewstr;
            ewstr.encode_from(*message);
            ewstr.copy_to(writer);
        }
        
    }
}