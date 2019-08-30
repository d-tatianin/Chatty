#pragma once

#include <vector>
#include <sstream>

#include "core/strings/wide_string.h"
#include "core/strings/encoded_wide_string.h"

namespace Chatty {

    namespace packet_type {

        enum id
        {
            BAD_PACKET    = -1,
            USER_REGISTER =  0, // [0] [USER_NAME]
            BEGIN_SESSION =  2, // [2] [OTHER_USER]
            ACCEPT        =  3, // [3]
            REJECT        =  4, // [4]
            CHAT_MESSAGE  =  5, // [5] [MESSAGE]
            CLOSE_SESSION =  6, // [6]
            OK_REPLY      =  7  // [7]
        };

        inline bool validate(id num)
        {
            return (
                num == USER_REGISTER ||
                num == BEGIN_SESSION ||
                num == ACCEPT        ||
                num == REJECT        ||
                num == CHAT_MESSAGE  ||
                num == CLOSE_SESSION ||
                num == OK_REPLY
            );
        }
    }

    class decoder
    {
    private:
        packet_type::id m_PacketType;
        wide_string m_Message;
    public:
        decoder(std::istream& response_stream);
        packet_type::id packet_type() { return m_PacketType; }
        wide_string& message() { return m_Message; }

        static void construct_packet(
            buffer& out_buffer, 
            packet_type::id type, 
            wide_string* message = nullptr
        );
    };
}