#include "ping.h"

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

void resolve_host(const char *host)
{
    struct addrinfo hints;
    struct addrinfo *res;
    int             ret;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;

    ret = getaddrinfo(host, NULL, &hints, &res);
    if (ret != 0)
        errx(EX_NOHOST, "unknown host: %s", host);

    struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, g_cfg.dest_ip, sizeof(g_cfg.dest_ip));
    memcpy(&g_cfg.dest_addr, addr, sizeof(g_cfg.dest_addr));

    strncpy(g_cfg.dest_hostname, host, NI_MAXHOST - 1);
    g_cfg.dest_hostname[NI_MAXHOST - 1] = '\0';

    freeaddrinfo(res);
}

void handle_sigint(init sig)
{
    (void)sig;
    g_running = 0;
}

double elapsed_ms(struct timeval *start, struct timeval *end)
{
    return ((end->tv_sec - start->tv_sec) * 1000.0 +
            (end->tv_usec - start->tv_usec) / 1000.0);
}