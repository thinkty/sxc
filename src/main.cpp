#include "main.hpp"

#include <iostream>
#include <cstring>

/**
 * Entry point of the application. Validate the arguments and start the appropr-
 * iate XMPP client/server.
 */
int main(int argc, char * argv[])
{
	// Must give an argument whether it is client, server, or help
	if (argc != 2 ||  strcmp(argv[1], FLAG_HELP) == 0)
	{
		print_usage(argv[0]);
		return 1;
	}

	if (strcmp(argv[1], FLAG_CLIENT) == 0)
	{
		// Start client
	}
	else if (strcmp(argv[1], FLAG_SERVER) == 0)
	{
		// Start server
	}
	else
	{
		// Unknown option
		print_usage(argv[0]);
		return 1;
	}
}

void print_usage(char * name)
{
	std::cerr << "Usage: " << name << " [OPTION]\n" << HELP_MSG << std::endl;
}
