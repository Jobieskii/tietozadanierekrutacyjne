#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_func.h"
#include "raw_struct.h"
#include "logger.h"

extern logger_function* logger;

char* get_line_suffix(size_t size, char s[size], FILE* stream) {
  s[0] = '\0';
  char* ret = fgets(s, size, stream);
  if (ret) {
    // check that line starts with 'cpu(n)'.
    if (strlen(s) > 3 && strncmp(s, "cpu", 3)) return 0;
    // and skip that part
    s += 3;
    if (s[0] != ' ') {
      char* e;
      strtoul(s, &e, 10);
      s = e;
    }
    ret = s;
    // replace newline with \0
    char* pos = strchr(s, '\n');
    if (pos) *pos = '\0';
    else ret = 0;
  }
  return ret;
}
// s must be null terminated
void fill_from_line(unsigned long long numbers[stat_count_],
                    char const* s,
                    int base) {
  char const* const t = s;
  char* next = 0;
  for (size_t i = 0; i < stat_count_; ++i) {
    numbers[i] = strtoull(s, &next, base);
    if (s == next || !s[0]) {
      logger("line from /proc/stat not fully read. (%s)\n", t);
      break;
    }
    s = next;
  }
}

size_t get_proc_no(FILE* stream) {
  char buf[256];
  size_t n = 0;
  while (get_line_suffix(256, buf, stream)) ++n;
  return n - 1;
}