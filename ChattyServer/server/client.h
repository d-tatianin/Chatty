#pragma once

#include <boost/asio.hpp>

#include "core/chatty_defines.h"
#include "core/chatty_types.h"

#include "core/strings/wide_string.h"

namespace Chatty {

    class client
    {
    private:
        Shared<ios>     m_IOS;
        socket          m_Socket;
        uint32_t        m_ID;
        wide_string     m_Name;
        buffer          m_ReadBuffer;
        buffer          m_WriteBuffer;
        bool            m_ActiveSession;
        wide_string     m_TalkingToName;
        uint32_t        m_TalkingTo;

        forward_handler      m_ForwardMessage;
        resolve_id_handler   m_ResolveID;
        init_session_handler m_InitSession;
        init_session_reply_handler m_ReplyWith;
    public:
        client(
            Shared<ios> ios, 
            socket&& client_socket, 
            uint32_t id, 
            forward_handler handler_forward,
            resolve_id_handler handler_resolve,
            init_session_handler handler_init,
            init_session_reply_handler handler_reply
        );

        void read();
        void on_read(const error_code& ec, size_t bytesTransferred);

        void send_to(wide_string& message);
        void on_sent(const error_code& ec);

        void request_session(uint32_t from);
        void on_session_requested(const error_code& ec);

        void session_reply(packet_type::id reply);
        void on_session_reply_sent(const error_code& ec);

        const wide_string& name() { return m_Name; }
        void set_name(const wide_string& name) { m_Name = name; }

        bool is_in_session() { return m_ActiveSession; }
        void enable_session() { m_ActiveSession = true; }
        void disable_session() { m_ActiveSession = false; }

        uint32_t chatter() { return m_TalkingTo; }
        void set_chatter(uint32_t id) { m_TalkingTo = id; }
        void set_chatter_name(const wide_string& name) { m_TalkingToName = name; }

        uint32_t id() { return m_ID; }
    };
}
