#ifndef __MAIN_H__
#define __MAIN_H__

enum s_flood {
    FLOOD_SYN,
    FLOOD_PING
};

struct s_options {
    char * target;
    enum s_flood flood_type;
	int verbose;
	int colour;
	int max_threads;
	int port;
	unsigned long long max_packets;
	char * source_ip;
};

extern struct s_options options;

#endif
