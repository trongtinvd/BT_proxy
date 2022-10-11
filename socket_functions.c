#include <string.h> 
#include <netdb.h>
#include <stdlib.h>
#include "socket_functions.h"
#include "string_functions.h"
#define BUFFER_SIZE 4096

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

    char buffer[BUFFER_SIZE];
    char* appendable_buffer = duplicate_string("", 0);

    do {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_receive = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        char* old_buffer = appendable_buffer;
        appendable_buffer = concat_string(appendable_buffer, strlen(appendable_buffer), buffer, bytes_receive);
        free(old_buffer);
    } while (strstr(appendable_buffer, "\r\n\r\n") < appendable_buffer);

    if(!http_is_ok(appendable_buffer)){
        return http_error_code(appendable_buffer);
    }
    else if(http_is_sized(appendable_buffer)){
        return recieve_sized_http_content(sock_fd, appendable_buffer, buffer, BUFFER_SIZE);
    }
    else if(http_is_chunked(appendable_buffer)){
        return recieve_chunked_http_content(sock_fd, appendable_buffer, buffer, BUFFER_SIZE);
    }
    else{
        return "Html response have neither Content-Size or Transport-Encoding. Error.";
    }
}

char* recieve_sized_http_content(int sock_fd, char* appendable_buffer, char* buffer, int buffer_size){
    int content_length = get_http_content_length(appendable_buffer);
    int header_length = get_http_header_length(appendable_buffer);
    int content_received = strlen(appendable_buffer) - header_length;

    while(content_received < content_length){
        memset(buffer, 0, buffer_size);
        int bytes_receive = recv(sock_fd, buffer, buffer_size, 0);
        char* old_buffer = appendable_buffer;
        appendable_buffer = concat_string(appendable_buffer, strlen(appendable_buffer), buffer, bytes_receive);
        free(old_buffer);
        content_received += bytes_receive;
    }

    return duplicate_string(appendable_buffer + header_length, strlen(appendable_buffer + header_length));
}

char* recieve_chunked_http_content(int sock_fd, char* appendable_buffer, char* buffer, int buffer_size){
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
            int bytes_receive = recv(sock_fd, buffer, buffer_size, 0);
            char* old_buffer = appendable_buffer;
            appendable_buffer = concat_string(appendable_buffer, strlen(appendable_buffer), buffer, bytes_receive);
            free(old_buffer);
        }
        result = concat_string(result, strlen(result), appendable_buffer + read_position, bytes_to_read);
        read_position += bytes_to_read;
    }
    return result;
}