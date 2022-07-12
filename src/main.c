#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threads.h>
#include "logger.h"
#include "file_thread.h"
#include "analyzer_thread.h"
#include "printer_thread.h"
#include "raw_struct.h"
#include "proc_struct.h"
#include "main.h"

extern logger_function* logger;

size_t get_proc_no(FILE* stream) {
  char buf[256];
  size_t n = 0;
  while (get_line_suffix(256, buf, stream)) ++n;
  return n - 1;
}
void state_init(State* s, size_t proc_no, FILE* stat) {
  raw_stats* raw_statsarr[2] = {
    create_raw_stats(proc_no, (mtx_t){0}),
    create_raw_stats(proc_no, (mtx_t){0}),
  };
  proc_stats* proc_statsarr[2] = {
    create_proc_stats(proc_no, (mtx_t){0}),
    create_proc_stats(proc_no, (mtx_t){0}),
  };
  State state = {
    .finished = 0,
    .stat = stat,
    .proc_no = proc_no
  };
  state.raw_stats[0] = raw_statsarr[0];
  state.raw_stats[1] = raw_statsarr[1];
  state.proc_stats[0] = proc_statsarr[0];
  state.proc_stats[1] = proc_statsarr[1];

  *s = state;
}


int main(int argc, char* argv[argc + 1]) {
  FILE* stat = fopen("/proc/stat", "r");
  if (!stat) {
    perror("Failed to read /proc/stat.");
    return EXIT_FAILURE;
  }
  if (argc > 1 && !strncmp(argv[1], "-V", 2)) {
    FILE* f = file_log_init(argv[0]);
    if (f) {
      logger = file_log;
    }
  } else {
    logger = no_log;
  }

  size_t proc_no = get_proc_no(stat);
  logger("Detected number of processors: %zu \n", proc_no);
  State state = {0};
  state_init(&state, proc_no, stat);
  logger("State initialized.\n");
  thrd_t thrd[3];
  logger("threads allocated.\n");
  
  thrd_create(&thrd[0], file_thread, &state);
  thrd_create(&thrd[1], anal_thread, &state);
  thrd_create(&thrd[2], print_thread, &state);

  thrd_join(thrd[0], 0);
  thrd_join(thrd[1], 0);
  thrd_join(thrd[2], 0);

  for (int i = 0; i < 2; ++i) {
    delete_raw_stats(state.raw_stats[i]);
    delete_proc_stats(state.proc_stats[i]);
  }
  fclose(stat);
  file_log_close();

  return EXIT_SUCCESS;
}

