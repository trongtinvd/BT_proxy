#include <string.h>
#include <stdlib.h>
#include "string_functions.h"

char* remove_scheme_from_url(const char* url){
    if(strstr(url, "http://") == url){
        url +=strlen("http://");
    } else if(strstr(url, "https://") == url){
        url +=strlen("https://");
    }

    char* result = malloc(strlen(url));
    strcpy(result, url);
    return result;
}

char* get_domain_from_url(const char* url){
    char* temp = remove_scheme_from_url(url);

    char* resource = strstr(temp, "/");
    if(resource != NULL){
        resource[0]='\0';
    }

    char* result = malloc(strlen(temp));
    strcpy(result, temp);
    free(temp);
    return result;
}

char* get_resource_from_url(const char* url){
    char* temp = remove_scheme_from_url(url);
    
    char* resource = strstr(temp, "/");
    char* result;

    if(resource == NULL){
        result = malloc(strlen("/"));
        strcpy(result, "/");
    }
    else{

        result = malloc(strlen(resource));
        strcpy(result, resource);
    }
    free(temp);
    return result;
}

char* generate_http_request(const char* domain, const char* resource){
    //"GET <resource> HTTP/1.1\r\nHost: <domain>\r\n\r\n"
    char* result = malloc(strlen("GET ") 
                            + strlen(resource) 
                            + strlen(" HTTP/1.1\r\nHost: ") 
                            + strlen(domain) 
                            + strlen("\r\n\r\n\0"));
    result[0] = '\0';

    strcat(result, "GET ");
    strcat(result, resource);
    strcat(result, " HTTP/1.1\r\nHost: ");
    strcat(result, domain);
    strcat(result, "\r\n\r\n\0");

    return result;
}

int get_http_content_length(const char* http_data){
    char* num_start = strstr(http_data, "Content-Length: ") + strlen("Content-Length: ");
    char* num_end = strstr(num_start, "\r\n\r\n");
    
    char num[10];
    memcpy(num, num_start, num_end - num_start);
    return atoi(num);
}

int get_http_header_length(const char* http_data){
    char* content = strstr(http_data, "\r\n\r\n") + 4;
    return content - http_data;
}