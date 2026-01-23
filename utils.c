#include "ping.h"

void print_man(void)
{
    printf("Usage\n  ping [options] <destination>\n\nOptions:\n <destination>      dns name or ip address\n -v                 verbose output\n  -V                 print version and exit\n\n IPv4 options:\n  -4                 use IPv4\n -b                 allow pinging broadcast\n -R                 record route\n    -T <timestamp>     define timestamp, can be one of <tsonly|tsandaddr|tsprespec>\n");
}