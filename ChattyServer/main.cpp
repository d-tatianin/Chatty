#include "core/chatty_utils.h"

#include "server/server.h"

int main(int argc, char** argv)
{
	unsigned short serverPort = 0;

	if (argc != 2 || (argc >= 2 && is_help(argv[1])))
	{
		print_help();
		return 1;
	}
	else
	{
		serverPort = atoi(argv[1]);

		if (!serverPort)
		{
			print_help();
			return 1;
		}
	}

	Chatty::server server(serverPort);
	server.run();

	return 0;
}
