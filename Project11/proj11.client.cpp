/*  Reid Harry
 *  Computer Project #11
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
    string hostName = "";
    unsigned short int portNum = 0;
    string fileName = "";

    // Get command line arguments
    if (argc != 4)
    {
        printf("Invalid number of arguments. Stopping execution.\n");
        exit(1);
    }
    else
    {
        hostName = (string)argv[1];
        portNum = atoi(argv[2]);
        fileName = (string)argv[3];
    }

    // Assign a socket
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("socket");
        exit(2);
    }

    // Get host server
    struct hostent* server = gethostbyname(hostName.c_str());
    if (server == NULL)
    {
        printf("Can't find host. Stopping execution.");
        exit(3);
    }

    // Construct and fill a sockaddr_in
    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(portNum);
    bcopy(server->h_addr, &saddr.sin_addr.s_addr, server->h_length);

    // Connect to server
    int stat = connect(sd, (struct sockaddr *) &saddr, sizeof(saddr));
    if (stat < 0)
    {
        perror("connect");
        exit(4);
    }

    // Send filename to server
    int nsend = send(sd, fileName.c_str(), strlen(fileName.c_str()), 0);
    if (nsend < 0)
    {
        perror("send");
        exit(5);
    }

    // Get either FAILED or OPEN from server
    char buffer[BUFFSIZE];
    int count = recv(sd, buffer, sizeof(buffer), 0);
    if (count < 0)
    {
        perror("recieve");
        exit(6);
    }
    if ((string)buffer != "OPEN")
    {
        perror("FAILED");
        exit(6);
    }
    
    // Send SEND to server
    string message = "SEND";
    nsend = send(sd, message.c_str(), strlen(message.c_str()), 0);
    if (nsend < 0)
    {
        perror("send 2");
        exit(7);
    }

    // Receive the file contents in 64 byte segments
    char recbuf[64];
    while(recv(sd, recbuf, sizeof(recbuf), 0))
    {
        write(1, recbuf, sizeof(recbuf));
    }

    // Close the file
    close(sd);
}
