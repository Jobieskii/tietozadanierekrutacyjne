#include "logger.h"


logger_function* logger;

int no_log(char const* _, ...) {
    return 0;
}
