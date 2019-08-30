#pragma once

#include "core/chatty_types.h"
#include "core/decoder/decoder.h"

namespace Chatty {

    class client
    {
    private:
        Shared<ios>  m_SharedIOS;
        socket       m_Socket;
        Unique<work> m_Work;
        thread_pool  m_ThreadPool;
        string_pair  m_IPort;
        bool         m_Running;
        bool         m_InSession;
        buffer       m_ReadBuffer;
        buffer       m_WriteBuffer;
    public:
        client(const string_pair& ip_port);
        void run();
    private:
        void connect();
        void on_connected(const error_code& ec);

        void registration(const std::wstring& name);
        void on_registered(const error_code& ec);
        
        void begin_session_with(const std::wstring& name);
        void on_session_begun(const error_code& ec);

        void session_accept();
        void on_session_accepted(const error_code& ec);

        void read();
        void on_read(const error_code& ec, size_t bytes_written);

        void send_message(const std::wstring& string);
        void on_message_sent(const error_code& ec);

        void workerThread(Shared<ios> ios);

        void shutdown();
    };
}
