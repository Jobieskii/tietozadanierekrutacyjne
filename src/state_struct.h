#ifndef STATE_H
#define STATE_H

#include <signal.h>
#include <stdio.h>

typedef struct State State;

#include "proc_struct.h"
#include "raw_struct.h"

struct State {
    proc_stats* proc_stats[2];
    raw_stats* raw_stats[2];
    volatile sig_atomic_t finished;
    FILE* stat;
    size_t proc_no;
};

void state_init(State* s, size_t proc_no, FILE* stat);
void state_delete(State* state);

#endif