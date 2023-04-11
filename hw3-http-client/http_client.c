
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/un.h>
#define BUFFER_SIZE 100


int socket_connect(char *host, in_port_t port){
    struct hostent *host_info;
    struct sockaddr_in addr;
    int ret;

    if((host_info = gethostbyname(host)) == NULL){
        herror("gethostbyname");
        exit(1);
    }
    int data_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memcpy(&addr.sin_addr, host_info->h_addr, host_info->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    
    ret = connect(data_socket, (struct sockkaddr *)&addr, sizeof(struct sockaddr_in));

    if(ret == -1){
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    return data_socket;
}

int main(int argc, char *argv[]){
    char buffer[BUFFER_SIZE];

    if(argc < 3){
        fprintf(stderr, "Usage: ./http_client <hostname> <port>\n");
        exit(1);
    }

    char src[] = "Host: ";
	char dest[512] = "GET / HTTP/1.1\r\n";
    strncat(dest, src, 512);
    char *testvar = strdup(argv[1]);
    strncat(dest, testvar, 512);
    char src1[] = "\r\n";
    strncat(dest, src1, 512);
    char src2[] = "Connection: Close\r\n";
    strncat(dest, src2, 512);
    char src3[] = "\r\n";
    strncat(dest, src3, 512);
	printf(dest);

    char *header = dest;
    int sock = socket_connect(argv[1], atoi(argv[2]));
    write(sock, header, strlen(header));
    bzero(buffer, BUFFER_SIZE);

    while(read(sock, buffer, BUFFER_SIZE - 1) != 0){
        fprintf(stderr, "%s", buffer);
        bzero(buffer, BUFFER_SIZE);
    }

    close(sock);
    return 0;
}