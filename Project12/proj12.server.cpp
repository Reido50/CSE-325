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
#include <sys/stat.h>
#include <fcntl.h>
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
    int gsn = getsockname( sd, (struct sockaddr *) &saddr, &saddr_size );
    if (gsn < 0)
    {
        perror("getsockname");
        exit(3);
    }
    unsigned short int port = ntohs( saddr.sin_port );
    char name[256];
    int ghn = gethostname(name, 256);
    if (ghn < 0)
    {
        perror("gethostname");
        exit(4);
    }
    struct hostent* host;
    host = gethostbyname(name);
    printf("\n%s %d\n", host->h_name, port);


    // Listen for client
    int lstat = listen(sd, 5);
    if (lstat < 0)
    {
        perror("listen");
        exit(5);
    }


    // Accept connection
    struct sockaddr_in caddr;
    unsigned int clen = sizeof(caddr);
    int comm_sd = accept(sd, (struct sockaddr*) &caddr, &clen);
    if (comm_sd < 0)
    {
        perror("accept");
        exit(6);
    }


    // Recieve filename from client
    char buffer[BUFFSIZE];
    int count = recv(comm_sd, buffer, sizeof(buffer), 0);
    if (count < 0)
    {
        perror("recieve filename");
        exit(7);
    }
    int file = open(buffer, O_RDONLY);
    string message = "OPEN";
    if (file < 0)
    {
        message = "FAILED";
    }
    int sstat = send(comm_sd, message.c_str(), strlen(message.c_str()), 0);
    if (sstat < 0)
    {
        perror("send");
        exit(8);
    }
    if (message == "FAILED")
    {
        perror("could not open file");
        exit(9);
    }


    // Recieve SEND message from client
    bzero(buffer, sizeof(buffer));
    count = recv(comm_sd, buffer, sizeof(buffer), 0);
    if (count < 0)
    {
        perror("recieve SEND");
        exit(10);
    }
    if ((string)buffer != "SEND")
    {
        perror("did not recieve SEND");
        exit(11);
    }


    // Send contents of file
    char sendbuf[64];
    while(1)
    {
        int rstat = read(file, sendbuf, sizeof(sendbuf));
        if (rstat < 1)
        {
            break;
        }
        sstat = send(comm_sd, sendbuf, rstat, 0);
        if (sstat < 0)
        {
            perror("send contents of file");
            exit(12);
        }
    }


    // Close the file
    close(comm_sd);
}