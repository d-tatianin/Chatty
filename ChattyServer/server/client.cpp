#include "client.h"

namespace Chatty {

	client::client(socket&& client_socket, uint32_t id)
		: m_Socket(std::forward<socket&&>(client_socket)),
		  m_ID(id),
		  m_MutableBuffer(m_Buffer.prepare(CHATTY_BUFFER_SIZE))
	{
		read();
	}

	void client::read()
	{
		m_Buffer.consume(m_Buffer.size() + 1);
		m_Socket.async_read_some(
			m_MutableBuffer, 
			std::bind(&client::on_read, 
				this, 
				std::placeholders::_1, 
				std::placeholders::_2)
		);
	}

	void client::on_read(const error_code& ec, size_t bytesTransferred)
	{
		m_Buffer.commit(bytesTransferred);
		std::istream response_stream(&m_Buffer);
		



	}

	void client::reply(const error_code& ec)
	{

	}

}
