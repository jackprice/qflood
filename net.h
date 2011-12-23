#ifndef __NET_H__
#define __NET_H__

struct pseudo_header {
	unsigned int source_address;
	unsigned int dest_address;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short tcp_length;
	struct tcphdr tcp;
};

int net_init ();
void * net_thread (void * ptr);

#endif