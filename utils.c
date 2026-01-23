void print_man()
{
    printf("Usage
              ping [options] <destination>

            Options:
              <destination>      dns name or ip address
              -v                 verbose output
              -V                 print version and exit

            IPv4 options:
              -4                 use IPv4
              -b                 allow pinging broadcast
              -R                 record route
              -T <timestamp>     define timestamp, can be one of <tsonly|tsandaddr|tsprespec>
            ");
}