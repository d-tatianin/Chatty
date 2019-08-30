#pragma once

#include <memory>
#include <list>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "core/chatty_types.h"
#include "core/decoder/decoder.h"

#include "client.h"

namespace Chatty {

    class server
    {
    private:
        Shared<ios>  m_SharedIOS;
        socket       m_Socket;
        acceptor     m_Acceptor;
        Unique<work> m_Work;
        thread_pool  m_ThreadPool;
        bool         m_Running;

        std::list<client> m_ActiveClients;
    public:
        server(unsigned short serverPort);
        void run();
        void shutdown();
    private:
        void accept_clients();
        void on_accepted(const error_code& ec);

        void forward_message(wide_string& message, uint32_t to);

        uint32_t name_to_id(const wide_string& name);
        wide_string id_to_name(uint32_t id);

        void init_session(uint32_t from, uint32_t with);

        void init_session_reply(packet_type::id reply, uint32_t to);

        void workerThread(Shared<ios> ios);
    };
}
