#include <stdlib.h>
#include "reader_thread.h"
#include "raw_struct.h"
#include "state_struct.h"
#include "file_func.h"
#include "logger.h"

extern logger_function* logger;



void fill_from_raw(FILE* stat, size_t proc_no,
                  unsigned long long stat_raw[proc_no + 1][stat_count_]) {
  char buf[256];
  for (size_t n = 0; n < proc_no + 1; ++n) {
    char* nbuf = get_line_suffix(sizeof buf, buf, stat);
    if (nbuf) {
      fill_from_line(stat_raw[n], nbuf, 10);
    }
  }
  return;
}

int reader_thread(void* arg) {
  State* state = arg;
  size_t i = 0;
  logger("Reading thread started.\n");
  while(!state->finished) {
    logger("Reading trying (%d)\n", i);
    mtx_lock(&state->raw_stats[i]->mtx);
    logger("Reading thread locked (%d).\n", i);
    freopen("/proc/stat", "r", state->stat);
    fill_from_raw(state->stat, state->proc_no, state->raw_stats[i]->raw_arr);

    mtx_unlock(&state->raw_stats[i]->mtx);
    logger("Reading thread unlocked (%d).\n", i);
    ++i;
    i %= 2;
  }
  return 0;
}

