#ifndef RAW_STRUCT_H
#define RAW_STRUCT_H

typedef struct raw_stats raw_stats;

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

#include <threads.h>
#include <stdlib.h>


struct raw_stats {
  mtx_t mtx;
  size_t proc_no;
  size_t size;
  unsigned long long raw_arr[][stat_count_];
};

raw_stats* raw_stats_create(size_t n, mtx_t new_mtx);

void raw_stats_delete(raw_stats* r);

#endif