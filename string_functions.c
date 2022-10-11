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
    char* num_end = strstr(num_start, "\r\n");
    
    char num[10];
    memcpy(num, num_start, num_end - num_start);
    return atoi(num);
}

int get_http_header_length(const char* http_data){
    char* content = strstr(http_data, "\r\n\r\n") + 4;
    return content - http_data;
}

_Bool http_is_sized(const char* http_data){
    char* content_length_position = strstr(http_data, "Content-Length: ");
    char* start_of_http_position = strstr(http_data, "\r\n\r\n");

    return content_length_position > http_data && content_length_position < start_of_http_position;
}

_Bool http_is_chunked(const char* http_data){
    char* transfer_encoding_position = strstr(http_data, "Transfer-Encoding: chunked");
    char* start_of_http_position = strstr(http_data, "\r\n\r\n");

    return transfer_encoding_position > http_data && transfer_encoding_position < start_of_http_position;
}

char* get_hex_length_header(const char* http_data){
    char* rn = strstr(http_data, "\r\n");
    int header_length = rn - http_data;
    if(header_length <= 0){
        return duplicate_string("", 0);
    }
    char* result = malloc(header_length + 1);
    strncpy(result, http_data, header_length);
    result[header_length] = 0;
    return result;
}

int hex_to_int(const char* text){
    int text_length = strlen(text);
    int result = 0;
    for(int i=0;i<text_length;i++){
        char c =text[i];
        int value;
        if(c >= '0' && c <= '9'){
            value = c - '0';
        }
        else if(c >= 'a' && c <= 'f'){
            value = c - 'a' + 10;
        }
        else if(c >= 'A' && c <= 'F'){
            value = c - 'A' + 10;
        }
        else{
            return result;
        }
        result = result * 16 + value;
    }
    return result;
}

char* concat_string(const char* text_1, const int text_1_length, const char* text_2, const int text_2_length){
    char* result = malloc(text_1_length + text_2_length + 1);
    strcpy(result, text_1);
    strncpy(result + text_1_length, text_2, text_2_length);
    result[text_1_length + text_2_length] = 0;
    return result;
}

char* duplicate_string(const char* text, int length){
    char* result = malloc(length + 1);
    strncpy(result, text, length);
    result[length] = 0;
    return result;
}

_Bool http_is_ok(const char* http_response){
    char* rn = strstr(http_response, "\r\n");
    char* code_200 = strstr(http_response, "200 OK");
    return code_200 > http_response && rn > code_200;
}

char* http_error_code(const char* http_response){
    char* rn = strstr(http_response, "\r\n");
    return duplicate_string(http_response, rn - http_response);
}