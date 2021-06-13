#ifndef MAIN_H
#define MAIN_H

#define FLAG_CLIENT "-c"
#define FLAG_SERVER "-s"
#define FLAG_HELP		"-h"
#define HELP_MSG		"A simple XMPP chat application.\n\nOptions\n\t-c\tstart application as client\n\t-s\tstart application as server\n\t-h\tdisplay this help and exit"

/**
 * Print application usage
 *
 * @param	name	Name of the program (= argv[0])
 */
void print_usage(char * name);

#endif
