#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "ft_ping: usage error: Destination address required");
        return(1);
    }
    if (argc == 3)
    {
        if (argv[2] == "-v")
        {
            return(0);
        }
    }
    if (argc == 2)
    {
        return(0);
    }
    print_man();
    return(2);
}