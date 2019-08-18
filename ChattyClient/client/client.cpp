#include "client.h"

#include <boost/asio.hpp>

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

	void client::workerThread(Shared<ios> ios)
	{
		ios->run();
	}
}