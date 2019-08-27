#pragma once

#include "core/chatty_types.h"
#include "core/decoder/decoder.h"

namespace Chatty {

    class client
    {
    private:
        Shared<ios>  m_SharedIOS;
        socket       m_Socket;
        Unique<work> m_Work;
        thread_pool  m_ThreadPool;
        bool         m_Running;
        buffer       m_ReadBuffer;
        buffer       m_WriteBuffer;
    public:
        client(const string_pair& ip_port);
        void run();
    private:
        void shutdown();

        void read();
        void on_read();

        void workerThread(Shared<ios> ios);
    };
}
