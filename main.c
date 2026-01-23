#include "ping.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "ft_ping: usage error: Destination address required\n");
        return(1);
    }
    else if (argc == 3)
    {
        if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[2], "-v") == 0))
        {
            printf("Verbose\n");
            return(0);
        }
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-?") == 0)
        {
            print_man();
            return(2);
        }
        ping(argv[1]);
        return(0);
    }
    fprintf(stderr, "ft_ping: usage error\n");
    return (1);
}