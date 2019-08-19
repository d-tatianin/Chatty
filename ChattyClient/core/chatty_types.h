#pragma once

#include <functional>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace Chatty {

	typedef std::pair<std::string, std::string>
		string_pair;
	typedef boost::asio::io_service
		ios;
	typedef boost::asio::io_service::work
		work;
	typedef boost::asio::ip::tcp::acceptor
		acceptor;
	typedef boost::asio::ip::tcp::socket
		socket;
	typedef boost::asio::ip::tcp::endpoint
		endpoint;
	typedef boost::thread_group
		thread_pool;
	typedef boost::system::error_code
		error_code;

	template<typename T>
	using Shared = typename std::shared_ptr<T>;
	template<typename T>
	using Unique = typename std::unique_ptr<T>;
}
