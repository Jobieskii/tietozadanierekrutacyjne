#ifndef FILE_THREAD_H
#define FILE_THREAD_H

enum stats {
  stat_user,
  stat_nice,
  stat_system,
  stat_idle,
  stat_iowait,
  stat_irq,
  stat_softirq,
  stat_steal,
  stat_guest,
  stat_guest_nice,

  stat_count_,
};

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* get_line_suffix(size_t size, char s[size], FILE* stream);
void fill_from_line(unsigned long long numbers[stat_count_],
                    char const* s,
                    int base);
void fill_from_raw(FILE* stat, size_t proc_no,
                  unsigned long long stat_raw[proc_no + 1][stat_count_]);

int file_thread(void*);


#include "logger.h"
#include "main.h"



#endif