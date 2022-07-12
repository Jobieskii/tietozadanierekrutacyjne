#ifndef MAIN_H
#define MAIN_H

typedef struct State State;

#include "proc_struct.h"
#include "raw_struct.h"

struct State {
    proc_stats* proc_stats[2];
    raw_stats* raw_stats[2];
    int finished;
    FILE* stat;
    size_t proc_no;
};

#endif