#ifndef _socket_functions_
#define _socket_functions_

struct sockaddr_in get_server_addr(sa_family_t protocol, int port, char* domain);

int send_data(int sock_fd, char* data);

char* receive_HTTP_content(int sock_fd);

char* recieve_sized_http_content(int sock_fd, char* appendable_buffer, char* buffer, int buffer_size);

char* recieve_chunked_http_content(int sock_fd, char* appendable_buffer, char* buffer, int buffer_size);

#endif