#ifndef PING_H
#define PING_H

# define _POSIX_C_SOURCE 200112L
# define _DEFAULT_SOURCE

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <signal.h>
# include <math.h>

# include <sys/socket.h>
# include <sys/time.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <sysexits.h>
# include <err.h>

#define PING_PACKET_SIZE    56
#define ICMP_HEADER_SIZE    8
#define IP_HEADER_SIZE      20
#define RECV_BUF_SIZE       1024
#define DEFAULTT_TTL        64
#define PING_INTERVAL       1

typedef struct s_ping_stats
{
    int     packets_sent;
    int     packets_recv;
    double  rtt_min;
    double  rtt_max;
    double  rtt_sum;
    double  rtt_sum_sq;
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
extern volatile int g_running;

void        print_man(void);
void        resolve_host(const char *host);
void        ping_loop(void);
double      elapsed_ms(struct timeval *start, struct timeval *end);
void        send_ping(void);
uint16_t    checksum(void *buf, int len);
void        recv_ping(void);
void        print_stats(void);
void        handle_sigint(int sig);
double      timeval_to_ms(struct timeval *tv);


#endif