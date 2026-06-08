#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <err.h>
#include <sys/time.h>

typedef struct s_ping_stats
{
    int     packets_sent;
    int     packets_recv;
    double  rtt_min;
    double  rtt_max;
    double  rtt_sum;
    double  stt_sum_sq;
}   t_ping_stats;

typedef struct s_ping_cfg
{
    int                 sockfd;
    int                 verbose;
    int                 ttl;
    char                *dest_str;
    char                dest_ip[INET_ADDRSTRLEN];
    char                dest_hostname[NI_MAXHOST];
    struct sockaddr_in  dest_addr;
    t_ping_stats        stats;
    int                 seq;
    pid_t               pid;
}   t_ping_cfg;

extern t_ping_cfg   g_cfg;

void print_man(void);

#endif