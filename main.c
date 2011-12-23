/******************************************************************************/
/**                                                                          **/
/**                                                                          **/
/**                                                                          **/
/**                                                                          **/
/**                                                                          **/
/**                                                                          **/
/******************************************************************************/
/** Copyright (C) 2011 Quetuo (http://www.quetuo.net)                        **/
/**                                                                          **/
/** This program is free software: you can redistribute it and/or modify     **/
/** it under the terms of the GNU General Public License as published by     **/
/** the Free Software Foundation, either version 3 of the License, or        **/
/** (at your option) any later version.                                      **/
/**                                                                          **/
/** This program is distributed in the hope that it will be useful,          **/
/** but WITHOUT ANY WARRANTY; without even the implied warranty of           **/
/** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **/
/** GNU General Public License for more details.                             **/
/**                                                                          **/
/** You should have received a copy of the GNU General Public License        **/
/** along with this program.  If not, see <http://www.gnu.org/licenses/>.    **/
/******************************************************************************/

#include "include.h"

struct s_options options;

int main (int argc, char * argv []) {
    /* CONFIGURATION */
    // First set defaults
    options.target = NULL;
    options.flood_type = FLOOD_SYN;
	options.verbose = 0;
	options.colour = 1;
	options.max_threads = 16;
	options.port = 0;
	options.max_packets = 1000000;
	options.source_ip = "255.255.255.255";

	// Now parse command-line options
	static struct option long_options [] = {
		{"verbose", no_argument, &(options.verbose), 1},
		{"target", required_argument, 0, 't'},
		{"max-packets", required_argument, 0, 'm'},
		{"source", required_argument, 0, 's'},
		{"help", no_argument, NULL, 'h'}
	};
	int c;
	while (1) {
		int option_index = 0;
		c = getopt_long (argc, argv, "hm:s:t:v", long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 0:
				break;
			case 'h':
				printf ("%s\n"
				        "By Quetuo <http://www.quetuo.net>\n"
				        "\n"
				        "Usage:\n"
				        "\t%s [OPTION]... --target=[TARGET]\n"
				        "Floods the specified target with packets - use with caution.\n"
				        "\n"
				        "Options:\n"
				        "    -v, --verbose           use verbose mode (more output)\n"
				        "    -t, --target            specify the target\n"
				        "    -m, --max-packets       maximum packets per second\n"
				        "    -s, --source            source of the packets\n"
				        "    -h, --help              display this help information\n"
				        "\n"
				        "Report bugs to quetuo@quetuo.net\n"
				        "Qflood home page: <http://www.quetuo.net/qflood/>\n",
				        PACKAGE_STRING, argv [0]);
				return EXIT_SUCCESS;
			case 'm':
				options.max_packets = atoi (optarg);
				break;
			case 's':
				options.source_ip = strdup (optarg);
			case 't':
				options.target = strdup (optarg);
				break;
			case 'v':
				options.verbose = 1;
				break;
			default:
				return EXIT_FAILURE;
		}
	}

	// Check validity of options!
	if (options.target == NULL) {
		printf ("No target specified!\n");
		return EXIT_FAILURE;
	}
	if (options.flood_type == FLOOD_SYN && getuid () != 0) {
		printf ("Qflood needs root to perform a SYN flood!\n");
		return EXIT_FAILURE;
	}

	// Tell the user what we're about to do
	printf ("Starting ");
	if (options.flood_type == FLOOD_SYN) {
		printf ("SYN");
	} else if (options.flood_type == FLOOD_PING) {
		printf ("ping");
	}
	printf (" flood on target %s\n", options.target);

	// Start the supervisor
	if (supervisor_init () != 0) {
		printf ("Could not start the supervisor!\n");
		return EXIT_FAILURE;
	}
	
	return EXIT_FAILURE;
}