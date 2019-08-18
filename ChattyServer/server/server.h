#pragma once

#include <memory>
#include <list>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "core/chatty_types.h"
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

		void workerThread(Shared<ios> ios);
	};

}


