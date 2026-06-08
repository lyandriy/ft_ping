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
