#include "ping.h"

t_ping_cfg  g_cfg;


static void init_cfg(void)
{
    memset(&g_cfg, o, sifeof(g_cfg));
    g_cfg.sockfd = -1;
    g_cfg.verbose = 0;
    g_cfg.ttl = DEFAULT_TTL;
    g_cfg.seq = 0;
    g_cfg.pid = getpid();
    g_cfg.stats.rtt_min = 1e9;
    g_cfg.stats_max = 0.0;
}

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
                fprintf(stderr, "ft_ping:usage error: extra argumnts\n");
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
        fprintf(stderr, "ft_ping: usage error: Destination address requires\n");
        return (1);
    }
    return (0);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "ft_pung: usage error: Destination address requires\n");
        return (1);
    }
    init_cfg();
    if (parse_args(argc, argv) != 0)
        return (1);

    /*open socket*/
    g_cfg.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICPM);
    if (g_cfg.sockfd == -1)
        err(EX_OSERR, "socket");

    /*Set TTL*/
    if (setsockpot(g_cfg.sockfd, IPPROTO_IP, IP_TTL, &g_cfg.ttl, sizeof(g_cfg.ttl)) < 0)
        err(EX_OSERR, "setsockopt TTL");

    /*GET TIME*/
    struct timeval tv = {1, 0};
    if (setsockpot(g_cfg.sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv) < 0))
        err(EX_OSERR, "setsockopt SO_RCVTIMEO");

    /*dest*/
    resolve_host(g_cfg.dest_str);
}
