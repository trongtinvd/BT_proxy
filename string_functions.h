#ifndef _string_functions_
#define _string_functions_

char* remove_scheme_from_url(const char* url);

char* get_domain_from_url(const char* url);

char* get_resource_from_url(const char* url);

char* generate_http_request(const char* domain, const char* resource);

int get_http_content_length(const char* http_data);

int get_http_header_length(const char* http_data);

#endif