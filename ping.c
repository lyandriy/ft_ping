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
    gettimeofday(tv, NULL);

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

void recv_ping(void)
{
    uint8_t             buf[RECV_BUF_SIZE];
    struct sockaddr_in  from;
    socklen_t           from_len;
    int                 bytes;
    struct  ip          *ip_hdr;
    struct icmphdr      *icmp;
    int                 ip_hdr_len;
    struct timeval      recv_time;
    struct timeval      *send_time;
    double              rtt;
    char                from_ip[INET_ADDRSTRLEN];

    gettimeofday(&recv_time, NULL);

    from_len = sizeof(from);
    bytes = recvfrom(g_cfg.sockfd, buf, sizeof(buf),  0,
                      (struct sockaddr *)&from, &from_len);
    if (bytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        if (g_cfg.verbose)
            warn("recvfrom");
        return;
    }

    ip_hdr = (struct ip *)buf;
    ip_hdr_len = ip_hdr->ip_hl * 4;

    if (bytes < ip_hdr_len + ICMP_HEADER_SIZE)
        return ;
    
    icmp = (struct icmphdr *)(buf + ip_hdr_len);

    inet_ntop(AF_INET, &from.sin_addr, from_ip, sizeof(from_ip));

    if (ntohs(icmp->un.echo.id) != (uint16_t)(g_cfg.pid & 0xffff))
        return ;
    
        send_time = (struct timeval *)(buf + ip_hdr_len + ICMP_HEADER_SIZE);
        rtt = elapsed_ms(send_time, & recv_time);

        g_cfg.stats.packets_recv++;
        if (rtt < g_cfg.stats.rtt_min)
            g_cfg.stats.rtt_min = rtt;
        if (rtt > g_cfg.stats.rtt_max)
            g_cfg.stats.rtt_max = rtt;
        g_cfg.stats.rtt_sum += rtt;
        g_cfg.stats.rtt_sum_sq += rtt * rtt;

        printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                bytes - ip_hdr_len, from_ip, ntohs(icmp->un.echo.sequence), ip_hdr->ip_ttl, rtt);
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