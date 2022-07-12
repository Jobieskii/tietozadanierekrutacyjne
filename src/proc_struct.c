#include "proc_struct.h"
#include "logger.h"

extern logger_function* logger;

proc_stats* proc_stats_create(size_t n, mtx_t new_mtx) {
    proc_stats* s = malloc (sizeof(mtx_t) + sizeof(size_t) + sizeof(double[n + 1]));
    if (s) {
        mtx_init(&new_mtx, mtx_plain);
        s->mtx = new_mtx;
        s->proc_no = n + 1;
    } else {
        logger("Couldn't allocate memory (proc_stats).");
        exit(EXIT_FAILURE);
    }
    return s;
}
void proc_stats_delete(proc_stats* r) {
    mtx_destroy(&r->mtx);
    free(r);
}
