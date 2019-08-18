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
		std::cout << "Server is live!" << std::endl;

		while (m_Running)
		{
			std::string command;
			std::cin >> command;

			if (command == "shutdown")
				shutdown();
			else if (command == "clrscr" || command == "clear" || command == "cls")
				CLEAR_SCREEN;
			else
				std::cout << "Unknown command!" << std::endl;
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
		if (m_Running)
		{
			std::cout << "A new client has just joined the server!" << std::endl;

			m_ActiveClients.emplace_back(std::move(m_Socket));

			if (m_Running)
				m_SharedIOS->post(std::bind(&server::accept_clients, this));
		}
	}

	void server::workerThread(Shared<ios> ios)
	{
		ios->run();
	}

	void server::shutdown()
	{
		std::cout << "Server shutting down..." << std::endl;
		m_Running = false;
		m_Work.reset();
		m_Acceptor.cancel();
	}
}
