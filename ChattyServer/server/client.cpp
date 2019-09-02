#include <iostream>

#include "client.h"

#include "core/decoder/decoder.h"

namespace Chatty {

    client::client(
        Shared<ios> ios,
        socket&& client_socket,
        uint32_t id,
        forward_handler handler_forward,
        resolve_id_handler handler_resolve,
        init_session_handler handler_init,
        init_session_reply_handler handler_reply
    )
        : m_IOS(ios),
        m_Socket(std::forward<socket&&>(client_socket)),
        m_ID(id),
        m_ActiveSession(false),
        m_TalkingTo(0),
        m_Name(),
        m_TalkingToName(),
        m_ForwardMessage(handler_forward),
        m_ResolveID(handler_resolve),
        m_InitSession(handler_init),
        m_ReplyWith(handler_reply)
    {
        m_IOS->post(std::bind(&client::read, this));
    }

    void client::read()
    {
        m_ReadBuffer.consume(m_ReadBuffer.size() + 1);
        m_Socket.async_read_some(
               m_ReadBuffer.prepare(CHATTY_BUFFER_SIZE),
            std::bind(&client::on_read, 
                this, 
                std::placeholders::_1, 
                std::placeholders::_2)
        );
    }

    void client::on_read(const error_code& ec, size_t bytesTransferred)
    {
        m_ReadBuffer.commit(bytesTransferred);
        std::istream response_stream(&m_ReadBuffer);
        decoder response(response_stream);

        switch (response.packet_type())
        {
        case packet_type::USER_REGISTER:
            m_Name = std::move(response.message());
            break;
        case packet_type::BEGIN_SESSION:
            m_TalkingToName = std::move(response.message());
            m_TalkingTo = m_ResolveID(m_TalkingToName);
            m_InitSession(m_ID, m_TalkingTo);
            break;
        case packet_type::CHAT_MESSAGE:
            if(m_ActiveSession)
                m_ForwardMessage(response.message(), m_TalkingTo);
            break;
        case packet_type::ACCEPT:
            m_ActiveSession = true;
            m_ReplyWith(response.packet_type(), m_TalkingTo);
            break;
        case packet_type::REJECT:
            m_ReplyWith(response.packet_type(), m_TalkingTo);
            m_TalkingTo = 0;
            break;
        }

        m_IOS->post(std::bind(&client::read, this));
    }

    void client::send_to(wide_string& message)
    {
        decoder::construct_packet(
            m_WriteBuffer, 
            packet_type::CHAT_MESSAGE, 
            &message
        );

        async_write(
            m_Socket, 
            m_WriteBuffer, 
            std::bind(
                &client::on_sent, 
                this, 
                std::placeholders::_1
            )
        );
    }

    void client::on_sent(const error_code& ec)
    {
        if (ec)
        {
            std::wcout << L"Error while sending a message to the client!" << std::endl;
            async_write(
                m_Socket, 
                m_WriteBuffer, 
                std::bind(
                    &client::on_sent, 
                    this, 
                    std::placeholders::_1
                )
            );
        }
    }

    void client::request_session(uint32_t from)
    {
        decoder::construct_packet(
            m_WriteBuffer, 
            packet_type::BEGIN_SESSION, 
            &m_TalkingToName
        );

        async_write(
            m_Socket, 
            m_WriteBuffer, 
            std::bind(
                &client::on_session_requested, 
                this, 
                std::placeholders::_1
            )
        );
    }

    void client::on_session_requested(const error_code& ec)
    {
        if (ec)
        {
            std::wcout << L"Error while sending a session request to the client!" << std::endl;
        }
    }

    void client::session_reply(packet_type::id reply)
    {
        decoder::construct_packet(m_WriteBuffer, reply);

        if (reply == packet_type::ACCEPT)
            m_ActiveSession = true;

        async_write(
            m_Socket, 
            m_WriteBuffer, 
            std::bind(
                &client::on_session_reply_sent, 
                this, 
                std::placeholders::_1
            )
        );
    }

    void client::on_session_reply_sent(const error_code& ec)
    {
        if (ec)
        {
            std::wcout << L"Error while sending a session request reply to the client!" << std::endl;
        }
    }
}
