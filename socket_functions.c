#include <string.h> 
#include <netdb.h>
#include <stdlib.h>
#include "socket_functions.h"
#include "string_functions.h"

struct sockaddr_in get_server_addr(sa_family_t protocol, int port, char* domain){

    struct sockaddr_in result;

    result.sin_family = protocol;
    result.sin_port = htons(port);

    struct hostent* addr_info = gethostbyname(domain);
    result.sin_addr.s_addr = *(u_int32_t*)*addr_info->h_addr_list;

    return result;
}

int send_data(int sock_fd, char* data){
    int bytes_to_send = strlen(data);
    int bytes_send = 0;

    while(bytes_send < bytes_to_send){
        int n = send(sock_fd, data, strlen(data), 0);
        if(n < 0){
            break;
        }
        bytes_send += n; 
    }

    return bytes_send;
}

char* receive_HTTP_content(int sock_fd){

    int buffer_size = 1024;
    char buffer[buffer_size];

    memset(buffer, 0, buffer_size);
    int n = recv(sock_fd, buffer, buffer_size, 0);
    buffer[n] = 0;
    int content_length = get_http_content_length(buffer);
    int header_length = get_http_header_length(buffer);
    int content_received = n - header_length;

    char* result = malloc(content_length + 1);
    strcpy(result, buffer + header_length);

    while(content_received < content_length){
        memset(buffer, 0, buffer_size);
        n = recv(sock_fd, buffer, buffer_size, 0);
        if(n < 0){
            break;
        }
        buffer[n] = 0;
        strcat(result, buffer);
        content_received += n;
    }

    return result;
}