#ifndef _string_functions_
#define _string_functions_

char* remove_scheme_from_url(const char* url);

char* get_domain_from_url(const char* url);

char* get_resource_from_url(const char* url);

char* generate_http_request(const char* domain, const char* resource);

int get_http_content_length(const char* http_data);

int get_http_header_length(const char* http_data);

_Bool http_is_sized(const char* http_data);

_Bool http_is_chunked(const char* http_data);

char* get_hex_length_header(const char* http_data);

int hex_to_int(const char* text);

char* concat_string(const char* text1, const char* text2, const int text2_length);

#endif