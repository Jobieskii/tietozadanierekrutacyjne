#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threads.h>
#include <signal.h>
#include <string.h>
#include "main.h"
#include "logger.h"
#include "reader_thread.h"
#include "analyzer_thread.h"
#include "printer_thread.h"
#include "state_struct.h"
#include "file_func.h"

extern logger_function* logger;

State state = {0};

void term_handler(int UNUSED(signum)) {
    state.finished = 1;
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

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term_handler;
    sigaction(SIGTERM, &action, NULL);

    size_t proc_no = get_proc_no(stat);
    logger("Detected number of processors: %zu \n", proc_no);
    state_init(&state, proc_no, stat);
    logger("State initialized.\n");
    thrd_t thrd[3];
    logger("threads allocated.\n");

    thrd_create(&thrd[0], reader_thread, &state);
    thrd_create(&thrd[1], anal_thread, &state);
    thrd_create(&thrd[2], print_thread, &state);

    thrd_join(thrd[0], 0);
    thrd_join(thrd[1], 0);
    thrd_join(thrd[2], 0);

    state_delete(&state);
    fclose(stat);
    logger("Program finished, closing log file...\n");
    file_log_close();

    return EXIT_SUCCESS;
}

