#include "core/chatty_utils.h"

#include "client/client.h"

#include <io.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    unsigned short serverPort = 0;
    Chatty::string_pair ip_port;

    if (argc != 2 || (argc >= 2 && is_help(argv[1])))
    {
        print_help();
        return 1;
    }
    else
    {
        if (!extract_ip_port(argv[1], ip_port))
        {
            print_help();
            return 1;
        }
    }

    Chatty::client client(ip_port);
    client.run();
    
    return 0;
}
