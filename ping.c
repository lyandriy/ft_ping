#include "ping.h"

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
        
    }
}