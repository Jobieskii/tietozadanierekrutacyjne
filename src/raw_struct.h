#ifndef RAW_STRUCT_H
#define RAW_STRUCT_H

typedef struct raw_stats raw_stats;


#include <threads.h>
#include <stdlib.h>
#include "file_thread.h"


struct raw_stats {
  mtx_t mtx;
  size_t proc_no;
  size_t size;
  unsigned long long raw_arr[][stat_count_];
};

raw_stats* create_raw_stats(size_t n, mtx_t new_mtx);

void delete_raw_stats(raw_stats* r);

#endif