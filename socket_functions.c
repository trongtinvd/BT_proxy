#include <string.h> 
#include <netdb.h>
#include <stdlib.h>
#include "socket_functions.h"
#include "string_functions.h"
#include <stdio.h>

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

    if(http_is_sized(buffer)){
        return recieve_sized_http_content(sock_fd, buffer, buffer_size);
    }
    else if(http_is_chunked(buffer)){
        return recieve_chunked_http_content(sock_fd, buffer, buffer_size);
    }
    else{
        return NULL;
    }
}

char* recieve_sized_http_content(int sock_fd, char* buffer, int buffer_size){
    int n = strlen(buffer);
    int content_length = get_http_content_length(buffer);
    int header_length = get_http_header_length(buffer);
    int content_received = n - header_length;

    char* result = duplicate_string(buffer + header_length, strlen(buffer + header_length));

    while(content_received < content_length){
        memset(buffer, 0, buffer_size);
        n = recv(sock_fd, buffer, buffer_size, 0);
        if(n <= 0){
            break;
        }
        buffer[n] = 0;
        strcat(result, buffer);
        content_received += n;
    }

    return result;
}

char* recieve_chunked_http_content(int sock_fd, char* buffer, int buffer_size){
    char* appendable_buffer = duplicate_string(buffer, strlen(buffer));
    int read_position = get_http_header_length(appendable_buffer);

    char* result = "";
    while(1){

        char* hex_header = get_hex_length_header(appendable_buffer + read_position);
        int hex_length = hex_to_int(hex_header);
        int bytes_to_read = hex_length + strlen("\r\n");
        read_position += strlen(hex_header) + strlen("\r\n");
        if(hex_length == 0){
            break;
        }

        while(strlen(appendable_buffer + read_position) <= bytes_to_read){
            memset(buffer, 0, buffer_size);
            recv(sock_fd, buffer, buffer_size, 0);
            char* old_buffer = appendable_buffer;
            appendable_buffer = concat_string(appendable_buffer, strlen(appendable_buffer), buffer, strlen(buffer));
            free(old_buffer);
        }
        result = concat_string(result, strlen(result), appendable_buffer + read_position, bytes_to_read);
        read_position += bytes_to_read;
    }
    return result;
}