#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace Chatty {

	typedef boost::asio::io_service
		ios;
	typedef boost::asio::io_service::work
		work;
	typedef boost::asio::ip::tcp::acceptor
		acceptor;
	typedef boost::asio::ip::tcp::endpoint
		endpoint;
	typedef boost::asio::ip::tcp::resolver
		resolver;
	typedef boost::asio::ip::tcp::socket
		socket;
	typedef boost::asio::ip::tcp::endpoint
		endpoint;
	typedef boost::thread_group
		thread_pool;
	typedef boost::system::error_code
		error_code;
	typedef boost::asio::streambuf
		buffer;
	typedef boost::asio::mutable_buffer
		mutable_buffer;
	
	template<typename T>
	using Shared = typename std::shared_ptr<T>;
	template<typename T>
	using Unique = typename std::unique_ptr<T>;
}
