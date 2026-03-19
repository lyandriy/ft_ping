#ifndef FT_PING_H
# define FT_PING_H

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

# define PING_PACKET_SIZE   56      /* data bytes, like real ping */
# define ICMP_HEADER_SIZE   8
# define IP_HEADER_SIZE     20
# define RECV_BUF_SIZE      1024
# define DEFAULT_TTL        64
# define PING_INTERVAL      1       /* seconds between packets */

typedef struct s_ping_stats
{
    int             packets_sent;
    int             packets_recv;
    double          rtt_min;
    double          rtt_max;
    double          rtt_sum;
    double          rtt_sum_sq;     /* for stddev */
}   t_ping_stats;

typedef struct s_ping_cfg
{
    int             sockfd;
    int             verbose;
    int             ttl;
    char            *dest_str;          /* original argument */
    char            dest_ip[INET_ADDRSTRLEN];
    char            dest_hostname[NI_MAXHOST];
    struct sockaddr_in  dest_addr;
    t_ping_stats    stats;
    int             seq;
    pid_t           pid;
}   t_ping_cfg;

/* globals */
extern t_ping_cfg   g_cfg;
extern volatile int g_running;

/* prototypes */
void    ping_loop(void);
void    send_ping(void);
void    recv_ping(void);
uint16_t checksum(void *buf, int len);
void    print_stats(void);
void    print_man(void);
void    handle_sigint(int sig);
void    resolve_host(const char *host);
double  timeval_to_ms(struct timeval *tv);
double  elapsed_ms(struct timeval *start, struct timeval *end);

#endif
