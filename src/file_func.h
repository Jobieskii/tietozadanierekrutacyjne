#include <stdio.h>
#include <stdlib.h>
#include "raw_struct.h"

char* get_line_suffix(size_t size, char s[size], FILE* stream);
void fill_from_line(unsigned long long numbers[stat_count_],
                    char const* s,
                    int base);

size_t get_proc_no(FILE* stream);
