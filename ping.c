#include "ping.h"

uint16_t checksum(void *buf, int len)
{
    uint16_t    *ptr;
    uint32_t    sum;

    ptr = buf;
    sum = 0;
    while (len > 1)
    {
        sum += *ptr++;
        len -= 2;
    }
    if (len == 1)
        sum += *(uint8_t *)ptr;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ((uint16_t)(~sum));
}

void send_ping(void)
{
    uint8_t         packet[ICMP_HEADER_SIZE + PING_PACKET_SIZE];
    struct icmphdr  *icmp;
    struct timeval  *tv;
    int             sent;

    memset(packet, 0, sizeof(packet));
    icmp = (struct icmphdr *)packet;

    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = htons((uint16_t)(g_cfg.pid & 0xffff));
    icmp->un.echo.sequence = htons((uint16_t)g_cfg.seq);

    tv = (struct timeval *)(packet + ICMP_HEADER_SIZE);
    getttimeofday(tv, NULL);

    for (int i = (int)sizeof(struct timeval); i < PING_PACKET_SIZE; i++)
        packet[ICMP_HEADER_SIZE + i] = (uint8_t)i;

    icmp->checksum = checksum(packet, sizeof(packet));

    sent = sendto(g_cfg.sockfd, packet, sizeof(packet), 0,
                  (struct sockaddr *)&g_cfg.dest_addr,
                  sizeof(g_cfg.dest_addr));
    if (sent < 0)
    {
        if (g_cfg.verbose)
            warn("sendto");
    }
    else
        g_cfg.stats.packets_sent++;
}

void ping(char *argv)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == -1)
    {
        err(EX_OSERR, "socket");
    }
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

void ping_loop(void)
{
    struct timeval  last_send;
    struct timeval  now;
    double          elapsed;
    
    memset(&last_send, 0, sizeof(last_send));

    while (g_running)
    {
        gettimeofday(&now, NULL);
        elapsed = elapsed_ms(&last_send, &now);
        
        if (elapsed >= PING_INTERVAL * 1000.0 || last_send.tv_sec == 0)
        {
            send_ping();
            g_cfg.seq++;
            gettimeofday(&last_send, NULL);
        }

        recv_ping();
    }
}