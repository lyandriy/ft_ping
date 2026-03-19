#include "ft_ping.h"

/* ------------------------------------------------------------------ */
/*  Time helpers                                                        */
/* ------------------------------------------------------------------ */
double timeval_to_ms(struct timeval *tv)
{
    return (tv->tv_sec * 1000.0 + tv->tv_usec / 1000.0);
}

double elapsed_ms(struct timeval *start, struct timeval *end)
{
    return ((end->tv_sec  - start->tv_sec)  * 1000.0 +
            (end->tv_usec - start->tv_usec) / 1000.0);
}

/* ------------------------------------------------------------------ */
/*  DNS resolution                                                      */
/* ------------------------------------------------------------------ */
void resolve_host(const char *host)
{
    struct addrinfo hints;
    struct addrinfo *res;
    int             ret;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_RAW;

    ret = getaddrinfo(host, NULL, &hints, &res);
    if (ret != 0)
        errx(EX_NOHOST, "unknown host: %s", host);

    /* Save IP string */
    struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, g_cfg.dest_ip, sizeof(g_cfg.dest_ip));
    memcpy(&g_cfg.dest_addr, addr, sizeof(g_cfg.dest_addr));

    /*
    ** Save hostname:
    ** - If input is already an IP, hostname == IP (no reverse DNS needed per subject).
    ** - If FQDN was given, keep the original string (no reverse DNS in packet return).
    */
    strncpy(g_cfg.dest_hostname, host, NI_MAXHOST - 1);
    g_cfg.dest_hostname[NI_MAXHOST - 1] = '\0';

    freeaddrinfo(res);
}

/* ------------------------------------------------------------------ */
/*  Signal handler                                                      */
/* ------------------------------------------------------------------ */
void handle_sigint(int sig)
{
    (void)sig;
    g_running = 0;
}

/* ------------------------------------------------------------------ */
/*  Statistics (printed on exit, like inetutils)                        */
/* ------------------------------------------------------------------ */
void print_stats(void)
{
    double avg;
    double stddev;
    int    loss;

    printf("\n--- %s ping statistics ---\n", g_cfg.dest_hostname);

    loss = 0;
    if (g_cfg.stats.packets_sent > 0)
        loss = (int)(100.0 * (g_cfg.stats.packets_sent - g_cfg.stats.packets_recv)
                     / g_cfg.stats.packets_sent);

    printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
           g_cfg.stats.packets_sent,
           g_cfg.stats.packets_recv,
           loss);

    if (g_cfg.stats.packets_recv > 0)
    {
        avg    = g_cfg.stats.rtt_sum / g_cfg.stats.packets_recv;
        stddev = sqrt(g_cfg.stats.rtt_sum_sq / g_cfg.stats.packets_recv
                      - avg * avg);
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               g_cfg.stats.rtt_min, avg, g_cfg.stats.rtt_max, stddev);
    }
}

/* ------------------------------------------------------------------ */
/*  Help / manual                                                       */
/* ------------------------------------------------------------------ */
void print_man(void)
{
    printf(
        "Usage: ft_ping [OPTION...] HOST ...\n"
        "Send ICMP ECHO_REQUEST packets to network hosts.\n"
        "\n"
        " Options valid for all request types:\n"
        "\n"
        "  -v                   verbose output\n"
        "\n"
        "  -?                   give this help list\n"
        "\n"
        "Mandatory arguments to long options are mandatory"
        " for short options too.\n"
    );
}
