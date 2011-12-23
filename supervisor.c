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

long long unsigned packets = 0;
int packet_size = 0;

int supervisor_init () {
	net_init ();
	packets = 0;
	while (1) {
		printf ("[");
		int i = 0;
		for (i = 0; i < packets * 20 / options.max_packets; i ++) {
			printf ("#");
		}
		for ( ; i < 20; i ++) {
			printf (" ");
		}
		packet_size = 200;
		printf ("] %llu packets / second (%f Mbit/s)\n", packets, ((double) packet_size * packets * 8) / 1048576.0);
		fflush (stdout);
		packets = 0;
		sleep (1);
	}
	return -1;
}