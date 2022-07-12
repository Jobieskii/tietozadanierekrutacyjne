#include "raw_struct.h"
#include "logger.h"

extern logger_function* logger;

raw_stats* raw_stats_create(size_t n, mtx_t new_mtx) {
  size_t size = sizeof(mtx_t) + 
                sizeof(size_t) + 
                sizeof(size_t) +
                sizeof(unsigned long long[n + 1][stat_count_]);
  raw_stats* s = malloc(size);
  if (s) {
    mtx_init(&new_mtx, mtx_plain);
    s->mtx = new_mtx;
    s->size = size;
    s->proc_no = n + 1;
  } else {
    logger("Couldn't allocate memory (raw_stats).");
    exit(EXIT_FAILURE);
  }
  return s;
}
void raw_stats_delete(raw_stats* r) {
  mtx_destroy(&r->mtx);
  free(r);
}