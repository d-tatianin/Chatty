#include <iostream>

#include <boost/asio.hpp>

#include "client.h"


namespace Chatty {

    client::client(const string_pair& ip_port)
        : m_SharedIOS(new ios),
        m_Socket(*m_SharedIOS),
        m_Work(new work(*m_SharedIOS)),
        m_Running(true)
    {
        for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
            m_ThreadPool.create_thread(std::bind(&client::workerThread, this, m_SharedIOS));

        m_Socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip_port.first), 25557));
    }

    void client::run()
    {
        while (m_Running)
        {
            std::string command;
            std::cin >> command;

            if (command == "shutdown")
                shutdown();
            if (command == "register")
            {
                std::cin >> command;
            }
            else
                std::cout << "Unknown command!" << std::endl;
        }

        m_ThreadPool.join_all();
    }

    void client::read()
    {
        m_ReadBuffer.consume(m_ReadBuffer.size() + 1);
        m_Socket.async_read_some(m_ReadBuffer.prepare(256), std::bind(&client::on_read, this, std::placeholders::_1, std::placeholders::_2));
    }

    void client::on_read(const error_code& rc, size_t bytes_written)
    {
        m_ReadBuffer.commit(bytes_written);
        std::istream message(&m_ReadBuffer);
        decoder decoded(message);

        switch (decoded.packet_type())
        {
        case packet_type::ACCEPT:
            std::cout << "Message accepted";
        }
    }

    void client::shutdown()
    {
        m_Running = false;
        m_Work.reset();
    }

    void client::workerThread(Shared<ios> ios)
    {
        ios->run();
    }
}
