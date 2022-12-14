#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include "string_functions.h"
#include "socket_functions.h"
#include "file_functions.h"
#define HTTP_PORT 80

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: ./1712816 <url> <output file>\r\n");
        return -1;
    }

    printf("\r\n//START//\r\n\r\n");

    char* url = argv[1];
    char* output_file = argv[2];
    char* domain = get_domain_from_url(url);
    char* resource = get_resource_from_url(url);
    char* http_request = generate_http_request(domain, resource);

    printf("domain: %s\r\n", domain);
    printf("resource: %s\r\n", resource);

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd<0){
        printf("Cannot create socket\r\n");
        return -1;
    }

    printf("Connecting to HTTP server\r\n");
    struct sockaddr_in server_addr = get_server_addr(AF_INET, HTTP_PORT, domain);
    int client_fd = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(client_fd < 0){
        printf("Cannot connect to HTTP server\r\n");
        close(sock_fd);
        return -1;
    }

    printf("Established connection. Fetching data...\r\n");

    send_data(sock_fd, http_request);
    char* data = receive_HTTP_content(sock_fd);
    if(data == NULL){
        printf("error reading http data");
        close(sock_fd);
        return -1;
    }

    printf("\r\n////////////////////////////////////////////\r\n");
    printf("\r\n%s\r\n", data);
    printf("\r\n////////////////////////////////////////////\r\n");

    printf("\r\nwriting to %s...\r\n", output_file);

    if(write_to_file(output_file, data)){
        printf("complete!\r\n");
    }
    else{
        printf("unable to write!\r\n");
    }

    close(sock_fd);    
    printf("\r\n//END//\r\n");
    return 0;
}