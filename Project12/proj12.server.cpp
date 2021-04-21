/*  Reid Harry
 *  Computer Project #12
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
using namespace std;

#define BUFFSIZE 4096

int main(int argc, char **argv)
{
    // Assign a socket
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("socket");
        exit(1);
    }


    // Bind socket to port
    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = 0;     // Let OS choose the port
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int b = bind(sd, (struct sockaddr *) &saddr, sizeof(saddr));
    if (b < 0)
    {
        perror("bind");
        exit(2);
    }


    // Print host and port
    socklen_t saddr_size = sizeof(saddr);
    getsockname( sd, (struct sockaddr *) &saddr, &saddr_size );
    unsigned short int port = ntohs( saddr.sin_port );
    char name[256];
    gethostname(name, 256);
    struct hostent* host;
    host = gethostbyname(name);
    printf("%s %d\n", host->h_name, port);
}