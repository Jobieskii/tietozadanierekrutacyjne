#include <stdio.h>
#include <stdlib.h>
#include "state_struct.h"
#include "raw_struct.h"
#include "proc_struct.h"

void state_init(State* s, size_t proc_no, FILE* stat) {
    raw_stats* raw_statsarr[2] = {
        raw_stats_create(proc_no, (mtx_t) {0}),
        raw_stats_create(proc_no, (mtx_t) {0}),
    };
    proc_stats* proc_statsarr[2] = {
        proc_stats_create(proc_no, (mtx_t) {0}),
        proc_stats_create(proc_no, (mtx_t) {0}),
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

void state_delete(State* state) {
    for (size_t i = 0; i < 2; ++i) {
        raw_stats_delete(state->raw_stats[i]);
        proc_stats_delete(state->proc_stats[i]);
    }
    state = 0;
}