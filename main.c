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
        int i = (intptr_t)resource - (intptr_t)url;
        url[i]='\0';
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

int main(int argc, char** argv){

    printf("//START//\r\n");

    if(argc != 3){
        printf("Usage: ./1712816 <url> <output file>\r\n");
        return -1;
    }

    char* url = argv[1];
    char* output_file = argv[2];
    char* domain = get_domain_from_url(url);
    char* resource = get_resource_from_url(url);

    printf("domain: %s\r\n", domain);
    printf("resource: %s\r\n", resource);

    printf("//END//\r\n");
/*
    int socketfd, clientfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0){
        printf("Cannot create socket\n");
        return -1; 
    }
    printf("Socket created. fd = %d \n", socketfd);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    memcpy(&server_address.sin_addr, inet_addr("example.com"), sizeof(in_addr_t));



    if (inet_pton(AF_INET, "27.0.12.186", &server_address.sin_addr)
        <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((clientfd = connect(socketfd, (struct sockaddr*)&server_address, sizeof(server_address))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char* message = strdup("GET /page HTTP/1.1\r\nHost: example.com\r\n\r\n");
    int n;

    send(socketfd, message, strlen(message), 0);
    n = read(socketfd, buffer, BUFFER_SIZE);
    buffer[n-1] = '\0';
    printf("%s\n", buffer);
*/
    return 0;
}