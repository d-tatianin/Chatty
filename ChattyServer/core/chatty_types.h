#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <functional>

#include "core/strings/wide_string.h"
#include "core/strings/encoded_wide_string.h"

namespace Chatty {

    namespace packet_type { enum id; }

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
    typedef std::vector<char>
        string;
    typedef std::function<void(wide_string&, uint32_t)>
        forward_handler;
    typedef std::function<uint32_t(const wide_string&)>
        resolve_id_handler;
    typedef std::function<void(uint32_t, uint32_t)>
        init_session_handler;
    typedef std::function<void(packet_type::id, uint32_t)>
        init_session_reply_handler;

    using boost::asio::async_write;

    template<typename T>
    using Shared = typename std::shared_ptr<T>;
    template<typename T>
    using Unique = typename std::unique_ptr<T>;
}
