#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define BUFFER_SIZE 1024
#define HTTP_PORT 80
#define HTTPS_PORT 443

char* remove_scheme_from_url(char* url){
    if(strstr(url, "http://") == url){
        url +=strlen("http://");
    } else if(strstr(url, "https://") == url){
        url +=strlen("https://");
    }

    char* result = malloc(strlen(url));
    strcpy(result, url);
    return result;
}

char* get_domain_from_url(char* url){
    url = remove_scheme_from_url(url);

    char* resource = strstr(url, "/");
    if(resource != NULL){
        resource[0]='\0';
    }

    char* result = malloc(strlen(url));
    strcpy(result, url);
    free(url);
    return result;
}

char* get_resource_from_url(char* url){
    url = remove_scheme_from_url(url);
    
    char* resource = strstr(url, "/");
    char* result;

    if(resource == NULL){
        result = malloc(strlen("/"));
        strcpy(result, "/");
    }
    else{

        result = malloc(strlen(resource));
        strcpy(result, resource);
    }
    free(url);
    return result;
}

char* generate_http_request(char* domain, char* resource){
    //"GET <resource> HTTP/1.1\r\nHost: <domain>\r\n\r\n"
    char* result = malloc(100);
    result[0] = '\0';

    strcat(result, "GET ");
    strcat(result, resource);
    strcat(result, " HTTP/1.1\r\nHost: ");
    strcat(result, domain);
    strcat(result, "\r\n\r\n");

    return result;
}

struct sockaddr_in get_server_addr(sa_family_t protocol, int port, char* domain){

    struct sockaddr_in result;

    result.sin_family = protocol;
    result.sin_port = htons(port);

    struct hostent* addr_info = gethostbyname(domain);
    result.sin_addr.s_addr = *(u_int32_t*)*addr_info->h_addr_list;

    return result;
}

void send_data(int sock_fd, char* data){
    send(sock_fd, data, strlen(data), 0);
}

int receive_data(int sock_fd, char* buffer, int size){
    return recv(sock_fd, buffer, size, 0);
}

int main(int argc, char** argv){

    printf("\r\n//START//\r\n");

    if(argc != 3){
        printf("Usage: ./1712816 <url> <output file>\r\n");
        return -1;
    }

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
    printf("Socket created: fd = %d\r\n", sock_fd);

    struct sockaddr_in server_addr = get_server_addr(AF_INET, HTTP_PORT, domain);
    printf("Connecting to HTTP server\r\n");

    int client_fd = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(client_fd < 0){
        printf("Cannot connect to HTTP/HTTPS server\r\n");
        return -1;
    }

    printf("Established connection. Fetching data...\r\n");

    send_data(sock_fd, http_request);
    char buffer[BUFFER_SIZE] = { 0 };
    int n = receive_data(sock_fd, buffer, BUFFER_SIZE);

    printf("\r\n////////////////////////////////////////////\r\n");
    printf("\r\n%s\r\n", buffer);
    printf("\r\n////////////////////////////////////////////\r\n");

    printf("\r\n//END//\r\n");
    return 0;
}