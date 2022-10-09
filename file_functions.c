#include "file_functions.h"
#include <stdio.h>

int write_to_file(const char* file_name, const char* data){
    FILE* file = fopen(file_name, "w");
    fprintf(file, "%s", data);
    fclose(file);
    return 1;
}