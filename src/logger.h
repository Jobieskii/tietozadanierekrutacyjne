#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef int logger_function(char const*, ...);

int no_log(char const* s, ...);


#endif