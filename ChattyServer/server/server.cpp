#include <iostream>

#include "server.h"

#include "core/chatty_types.h"
#include "core/chatty_defines.h"
#include "core/chatty_utils.h"

namespace Chatty {

    server::server(unsigned short serverPort)
        : m_SharedIOS(new ios),
        m_Socket(*m_SharedIOS),
        m_Acceptor(*m_SharedIOS, get_endpoint(serverPort)),
        m_Work(new work(*m_SharedIOS)),
        m_Running(true)
    {
        for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
            m_ThreadPool.create_thread(std::bind(&server::workerThread, this, m_SharedIOS));
    }

    void server::run()
    {
        m_SharedIOS->post(std::bind(&server::accept_clients, this));
        std::wcout << L"Server is live!" << std::endl;

        while (m_Running)
        {
            std::wstring command;
            std::wcin >> command;
            
            if (command == L"shutdown")
                shutdown();
            else if (command == L"clrscr" || command == L"clear" || command == L"cls")
                CLEAR_SCREEN;
            else
                std::wcout << L"Unknown command!" << std::endl;
        }

        m_ThreadPool.join_all();
    }

    void server::accept_clients()
    {
        m_Acceptor.async_accept(m_Socket, 
            std::bind(&server::on_accepted, this, std::placeholders::_1)
        );
    }

    void server::on_accepted(const error_code& ec)
    {
        static uint32_t unique_id = 0;
        
        if (m_Running)
        {
            std::wcout << L"A new client has just joined the server!" << std::endl;

            m_ActiveClients.emplace_back(
                m_SharedIOS, 
                std::move(m_Socket), 
                ++unique_id, 
                std::bind(
                    &server::forward_message,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                ),
                std::bind(
                    &server::name_to_id,
                    this,
                    std::placeholders::_1
                ),
                std::bind(
                    &server::init_session,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                ),
                std::bind(
                    &server::init_session_reply,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );

            if (m_Running)
                m_SharedIOS->post(std::bind(&server::accept_clients, this));
        }
    }

    void server::forward_message(wide_string& message, uint32_t to)
    {
           auto recipient = std::find_if(m_ActiveClients.begin(), m_ActiveClients.end(), [=](client& c) { return c.id() == to; });

           recipient->send_to(message);
    }

    uint32_t server::name_to_id(const wide_string& name)
    {
        auto client_itr = std::find_if(m_ActiveClients.begin(), m_ActiveClients.end(), [&](client& c) { return c.name() == name; });

        if (client_itr != m_ActiveClients.end())
        {
            return client_itr->id();
        }

        return 0;
    }

    wide_string server::id_to_name(uint32_t id)
    {
        auto recipient = std::find_if(m_ActiveClients.begin(), m_ActiveClients.end(), [=](client& c) { return c.id() == id; });

        return recipient->name();
    }

    void server::init_session(uint32_t from, uint32_t with)
    {
        auto recipient = std::find_if(m_ActiveClients.begin(), m_ActiveClients.end(), [=](client& c) { return c.id() == with; });

        recipient->set_chatter(from);
        recipient->set_chatter_name(id_to_name(from));
        recipient->request_session(from);
    }

    void server::init_session_reply(packet_type::id reply, uint32_t to)
    {
        auto recipient = std::find_if(m_ActiveClients.begin(), m_ActiveClients.end(), [=](client& c) { return c.id() == to; });
        recipient->session_reply(reply);
    }

    void server::workerThread(Shared<ios> ios)
    {
        ios->run();
    }

    void server::shutdown()
    {
        std::wcout << "Server shutting down..." << std::endl;
        m_Running = false;
        m_Work.reset();
        m_Acceptor.cancel();
    }
}
