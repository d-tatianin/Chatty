#include "client.h"

namespace Chatty {

	client::client(socket&& client_socket)
		: m_Socket(std::forward<socket&&>(client_socket))
	{

	}

}