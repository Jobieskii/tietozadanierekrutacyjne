#include <stdarg.h>
#include "logger.h"
#include "main.h"


logger_function* logger;

int no_log(char const* _, ...) {
    return 0;
}

FILE* logfile = {0};

int file_log(char const* format, ...) {
    char f[1024];
    int done;
    va_list arg;
    va_start(arg, format);
    done = vsprintf(f, format, arg);
    va_end(arg);
    if (done) {
        fputs(f, logfile);
    }
    return done;
}

FILE* file_log_init(char* name) {
    char filename[256];
    sprintf(filename, "./%s.log", name);
    logfile = fopen(filename, "w+");
    return logfile;
}

void file_log_close() {
    if (logfile) fclose(logfile);
}