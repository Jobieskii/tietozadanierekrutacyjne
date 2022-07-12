#ifndef PROC_STRUCT_H
#define PROC_STRUCT_H

#include <threads.h>
#include <stdlib.h>

typedef struct processed_stats proc_stats;
struct processed_stats {
  mtx_t mtx;
  size_t proc_no;
  double processed_arr[];
};

proc_stats* proc_stats_create(size_t n, mtx_t new_mtx);

void proc_stats_delete(proc_stats* r);

#endif