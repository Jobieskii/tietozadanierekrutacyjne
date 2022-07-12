#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef int logger_function(char const*, ...);

int no_log(char const* s, ...);

int file_log(char const* s, ...);

FILE* file_log_init(char* name);
void file_log_close();

#endif