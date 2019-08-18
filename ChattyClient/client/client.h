#pragma once

#include "core/chatty_types.h"

namespace Chatty {

	class client
	{
	private:
		Shared<ios>  m_SharedIOS;
		socket       m_Socket;
		Unique<work> m_Work;
		thread_pool  m_ThreadPool;
		bool         m_Running;
	public:
		client(const string_pair& ip_port);
	private:
		void workerThread(Shared<ios> ios);
	};
}

