#include <unistd.h>
#include "main.h"
#include "logger.h"

extern logger_function* logger;

void print_usage(size_t proc_no, double arr[proc_no + 1]) {
    puts("\e[1;1H\e[2J");
    printf("total CPU usage: %.2f%%\n", arr[0] * 100.0);
    for (size_t i = 1; i < proc_no + 1; ++i) {
        printf("CPU%zu usage: %.2f%%\n", i - 1, arr[i] * 100.0);
    }
}


int print_thread(void* arg) {
  State* state = arg;
  size_t i = 0;
  logger("Printer thread started.\n");
  while(!state->finished) {
    logger("Printer trying (%d)\n", i);
    mtx_lock(&state->proc_stats[i]->mtx);
    logger("Printer thread locked (%d).\n", i);
    print_usage(state->proc_no, state->proc_stats[i]->processed_arr);
    
    thrd_sleep(&(struct timespec){.tv_sec=1}, 0);
    mtx_unlock(&state->proc_stats[i]->mtx);
    logger("Printer thread unlocked (%d).\n", i);
    ++i;
    i %= 2;
  }

  return 0;
}