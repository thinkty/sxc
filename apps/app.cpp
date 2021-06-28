#include <sxc/client.hpp>
#include <sxc/server.hpp>

#include <iostream>
#include <cstring>

#define FLAG_CLIENT "-c"
#define FLAG_SERVER "-s"
#define FLAG_HELP		"-h"
#define HELP_MSG		"A simple XMPP chat application.\n\nOptions\n\t-c\tstart application as client\n\t-s\tstart application as server\n\t-h\tdisplay this help and exit"

void print_usage(char * name);

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
		client();
	}
	else if (strcmp(argv[1], FLAG_SERVER) == 0)
	{
		// Start server
		server();
	}
	else
	{
		// Unknown option
		print_usage(argv[0]);
		return 1;
	}
}

/**
 * Print application usage
 *
 * @param	name	Name of the program (= argv[0])
 */
void print_usage(char * name)
{
	std::cerr << "Usage: " << name << " [OPTION]\n" << HELP_MSG << std::endl;
}
