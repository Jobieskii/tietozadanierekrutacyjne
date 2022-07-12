#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "raw_struct.h"
#include "state_struct.h"
#include "analyzer_thread.h"

extern logger_function* logger;
/*
Using formula from:
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
*/
void process_raw(size_t proc_no,
                 unsigned long long raw[proc_no + 1][stat_count_],
                 unsigned long long old[proc_no + 1][stat_count_],
                 double proc[proc_no]) {

    typedef unsigned long long ull;
    for (size_t i = 0; i < proc_no + 1; ++i) {
        ull prev_idle = old[i][stat_idle] + old[i][stat_iowait];
        ull idle = raw[i][stat_idle] + raw[i][stat_iowait];
        ull prev_nonidle =  old[i][stat_user] +
                            old[i][stat_nice] +
                            old[i][stat_system] +
                            old[i][stat_irq] +
                            old[i][stat_softirq] +
                            old[i][stat_steal];
        ull nonidle =   raw[i][stat_user] +
                        raw[i][stat_nice] +
                        raw[i][stat_system] +
                        raw[i][stat_irq] +
                        raw[i][stat_softirq] +
                        raw[i][stat_steal];
        ull prev_total = prev_idle + prev_nonidle;
        ull total = idle + nonidle;
        ull totald = total - prev_total;
        ull idled = idle - prev_idle;
        if (totald)
            proc[i] = (totald - idled) / (double) totald;
        else
            proc[i] = 0.0;
    }
}


int anal_thread(void* arg) {
    State* state = arg;
    size_t i = 0;
    unsigned long long old[state->proc_no+1][stat_count_];
    logger("Analyzer thread started.\n");
    while(!state->finished) {
        logger("Analyzer trying (%d)\n", i);
        mtx_lock(&state->raw_stats[i]->mtx);
        mtx_lock(&state->proc_stats[i]->mtx);
        logger("Analyzer thread locked (%d).\n", i);

        process_raw(state->proc_no, state->raw_stats[i]->raw_arr, old, state->proc_stats[i]->processed_arr);
        logger("Analyzer processed raw data.\n");
        memcpy(old, state->raw_stats[i]->raw_arr, sizeof(unsigned long long[state->proc_no + 1][stat_count_]));
        logger("Analyzer copied new data to old.\n");
        mtx_unlock(&state->raw_stats[i]->mtx);
        mtx_unlock(&state->proc_stats[i]->mtx);
        logger("Analyzer thread unlocked (%d).\n", i);
        ++i;
        i %= 2;
    }
    return 0;
}
