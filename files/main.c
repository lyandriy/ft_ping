#include "ft_ping.h"

t_ping_cfg      g_cfg;
volatile int    g_running = 1;

static void init_cfg(void)
{
    memset(&g_cfg, 0, sizeof(g_cfg));
    g_cfg.sockfd = -1;
    g_cfg.verbose = 0;
    g_cfg.ttl = DEFAULT_TTL;
    g_cfg.seq = 0;
    g_cfg.pid = getpid();
    g_cfg.stats.rtt_min = 1e9;
    g_cfg.stats.rtt_max = 0.0;
}

/*
** Very simple argument parser.
** Supported: -v, -?
** Destination must be present (not starting with '-').
** We replicate inetutils-2.0 behavior for error messages.
*/
static int parse_args(int argc, char **argv)
{
    int i;
    int dest_found;

    i = 1;
    dest_found = 0;
    while (i < argc)
    {
        if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_man();
            exit(2);
        }
        else if (strcmp(argv[i], "-v") == 0)
            g_cfg.verbose = 1;
        else if (argv[i][0] != '-')
        {
            if (dest_found)
            {
                fprintf(stderr, "ft_ping: usage error: extra arguments\n");
                return (1);
            }
            g_cfg.dest_str = argv[i];
            dest_found = 1;
        }
        else
        {
            fprintf(stderr, "ft_ping: invalid option -- '%s'\n", argv[i]);
            fprintf(stderr, "Try 'ft_ping -?' for more information.\n");
            return (1);
        }
        i++;
    }
    if (!dest_found)
    {
        fprintf(stderr, "ft_ping: usage error: Destination address required\n");
        return (1);
    }
    return (0);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "ft_ping: usage error: Destination address required\n");
        return (1);
    }
    init_cfg();
    if (parse_args(argc, argv) != 0)
        return (1);

    /* Open raw socket — requires root */
    g_cfg.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (g_cfg.sockfd == -1)
        err(EX_OSERR, "socket");

    /* Set TTL */
    if (setsockopt(g_cfg.sockfd, IPPROTO_IP, IP_TTL,
                   &g_cfg.ttl, sizeof(g_cfg.ttl)) < 0)
        err(EX_OSERR, "setsockopt TTL");

    /* Set receive timeout */
    struct timeval tv = {1, 0};
    if (setsockopt(g_cfg.sockfd, SOL_SOCKET, SO_RCVTIMEO,
                   &tv, sizeof(tv)) < 0)
        err(EX_OSERR, "setsockopt SO_RCVTIMEO");

    /* Resolve destination */
    resolve_host(g_cfg.dest_str);

    /* Setup signal handler */
    signal(SIGINT, handle_sigint);

    /* Print header like inetutils */
    printf("PING %s (%s): %d data bytes",
           g_cfg.dest_hostname, g_cfg.dest_ip, PING_PACKET_SIZE);
    if (g_cfg.verbose)
        printf(", id 0x%04x = %d", (unsigned)g_cfg.pid & 0xffff,
               (unsigned)g_cfg.pid & 0xffff);
    printf("\n");

    /* Main loop */
    ping_loop();

    /* Statistics */
    print_stats();

    close(g_cfg.sockfd);
    return (g_cfg.stats.packets_recv == 0 ? 1 : 0);
}
