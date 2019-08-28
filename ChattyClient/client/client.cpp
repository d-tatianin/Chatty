#include <iostream>

#include <boost/asio.hpp>

#include "client.h"


namespace Chatty {

    client::client(const string_pair& ip_port)
        : m_SharedIOS(new ios),
        m_Socket(*m_SharedIOS),
        m_Work(new work(*m_SharedIOS)),
        m_IPort(ip_port),
        m_Running(true),
        m_InSession(false)
    {
        for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
            m_ThreadPool.create_thread(std::bind(&client::workerThread, this, m_SharedIOS));

        m_SharedIOS->post(
            std::bind(
                &client::run,
                this
            )
        );
    }

    void client::connect()
    {

        m_Socket.async_connect(
            endpoint(
                boost::asio::ip::address::from_string(m_IPort.first),
                atoi(m_IPort.second.c_str())
            ),
            std::bind(
                &client::on_connected,
                this,
                std::placeholders::_1
            )
        );
        //async_connect(
        //    m_Socket,
        //    endpoint(
        //        boost::asio::ip::address::from_string(m_IPort.first),
        //        atoi(m_IPort.second.c_str())
        //    ),
        //    std::bind(
        //        &client::on_connected,
        //        this,
        //        std::placeholders::_1
        //    )
        //);
    }

    void client::on_connected(const error_code& ec)
    {
        if (ec)
        {
            std::cout << "Could not connect to the server. Reason: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Successfully connected." << std::endl;

            m_SharedIOS->post(
                std::bind(
                    &client::read,
                    this
                )
            );
        }
    }

    void client::run()
    {
        while (m_Running)
        {
            std::string command;
            std::getline(std::cin, command);

            if (command == "/shutdown")
                shutdown();
            else if (command.find("/register") != std::string::npos)
            {
                command.erase(command.begin(), std::find(command.begin(), command.end(), ' ') + 1);
                registration(command);
            }
            else if (command.find("/begin-session") != std::string::npos)
            {
                command.erase(command.begin(), std::find(command.begin(), command.end(), ' ') + 1);
                begin_session_with(command);
            }
            else if (command == "/accept")
            {
                m_InSession = true;
                session_accept();
            }
            else if (command == "/reconnect")
            {
                connect();
            }
            else if (command == "/clear")
            {
                system("cls");
            }
            else
            {
                send_message(command);
            }
        }

        m_ThreadPool.join_all();
    }

    void client::send_message(const std::string& string)
    {
        m_WriteBuffer.consume(m_WriteBuffer.size() + 1);
        std::ostream packet(&m_WriteBuffer);
        packet << packet_type::CHAT_MESSAGE;
        packet << string;
        async_write(m_Socket, m_WriteBuffer, std::bind(&client::on_message_sent, this, std::placeholders::_1));
    }
    void client::on_message_sent(const error_code& ec)
    {
        if (ec)
        {
            std::cout << "Could not send the message! Reason: " << ec.message() << std::endl;
        }
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
        case packet_type::BEGIN_SESSION:
            std::cout << decoded.message().data() << " wants to begin a session" << std::endl;
            break;
        case packet_type::CHAT_MESSAGE:
            std::cout << decoded.message().data() << std::endl;
            break;
        case packet_type::ACCEPT:
            std::cout << "Session accepted!" << std::endl;
            break;
        case packet_type::REJECT:
            std::cout << "Session rejected!" << std::endl;
            break;
        }

        m_SharedIOS->post(
            std::bind(
                &client::read,
                this
            )
        );
    }

    void client::registration(const std::string& name)
    {
        m_WriteBuffer.consume(m_WriteBuffer.size() + 1);
        std::ostream packet(&m_WriteBuffer);
        packet << packet_type::USER_REGISTER;
        packet << name;
        async_write(m_Socket, m_WriteBuffer, std::bind(&client::on_registered, this, std::placeholders::_1));
    }

    void client::on_registered(const error_code& ec)
    {
        if (ec)
        {
            std::cout << "Registration failed! Reason: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Successfully registered on the server!" << std::endl;
        }
    }

    void client::begin_session_with(const std::string& name)
    {
        m_WriteBuffer.consume(m_WriteBuffer.size() + 1);
        std::ostream packet(&m_WriteBuffer);
        packet << packet_type::BEGIN_SESSION;
        packet << name;
        async_write(m_Socket, m_WriteBuffer, std::bind(&client::on_session_begun, this, std::placeholders::_1));
    }

    void client::on_session_begun(const error_code& ec)
    {
        if (ec)
        {
            std::cout << "Could not begin session! Reason: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Successfully sent a session begin request!" << std::endl;
        }
    }

    void client::session_accept()
    {
        m_WriteBuffer.consume(m_WriteBuffer.size() + 1);
        std::ostream packet(&m_WriteBuffer);
        packet << packet_type::ACCEPT;
        async_write(m_Socket, m_WriteBuffer, std::bind(&client::on_session_accepted, this, std::placeholders::_1));
    }

    void client::on_session_accepted(const error_code& ec)
    {
        if (ec)
        {
            std::cout << "Could not accept session! Reason: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Successfully sent a session accepted request!" << std::endl;
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
